#include <iostream>
#include <stb_image.h>
#include "height_map.h"
#include "mesh_generator.h"
#include "obj_writer.h"

int main(int argc, char **argv) {

    using namespace TerrainToObj;

    auto heightmap = std::make_unique<HeightMap>("heightmap.png");
    MeshGenerator generator(std::move(heightmap), 0.0001f, 100000, 1000);

    generator.Generate();

    auto mesh = generator.GetMesh(Float3(1.0f, 1.0f, 3000.0f));

    ObjWriter::Write("output.obj", mesh.Points, mesh.Triangles);
}
