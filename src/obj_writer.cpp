#include "obj_writer.h"
#include <sstream>
#include <fstream>

namespace TerrainToObj
{
    bool ObjWriter::Write(const std::string& file, 
        const std::vector<Float3>& points, 
        const std::vector<Int3>& triangles)
    {
        std::ofstream of("output.obj");
        of << "# Terrain OBJ File " << std::endl;
        for (auto& v : points)
        {
            of << "v " << v.x() << " " << v.y() << " " << v.z() << " " << std::endl;
        }

        // triangles
        for (auto& t : triangles)
        {
            auto p1 = t.x()+1;
            auto p2 = t.y()+1;
            auto p3 = t.z()+1;
            
            of << "f " << p1 << "/" << p1 << " " 
                        << p2 << "/" << p2 << " "
                        << p3 << "/" << p3 << std::endl;

        }
        of.flush();

        return true;
    }
}
