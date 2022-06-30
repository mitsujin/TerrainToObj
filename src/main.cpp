#include <iostream>
#include <stb_image.h>
#include "height_map.h"
#include "mesh_generator.h"
#include "obj_writer.h"
#include <chrono>

int main(int argc, char **argv) {

    using namespace TerrainToObj;

    auto start = std::chrono::steady_clock::now();
    auto heightmap = std::make_unique<HeightMap>("heightmap.png");
//
//    auto heightmap = std::make_unique<HeightMap>(1, 2048);
 //   heightmap->SetData(0, "heightmap.png");
//    heightmap->SetData(1, "heightmap.png");
 //   heightmap->SetData(2, "heightmap.png");
  //  heightmap->SetData(3, "heightmap.png");

    MeshGenerator generator(std::move(heightmap), 0.000999f, 0, 0);

    generator.Generate();

    auto mesh = generator.GetMesh(Float3(1.0f, 1.0f, 3000.0f));

    auto end = std::chrono::steady_clock::now();
    auto took = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
    std::cout << "Took: " << took.count() << " seconds" <<  std::endl;
    std::cout << "Points: " << mesh.Points.size() << ", triangles: " << mesh.Triangles.size() << std::endl;
    ObjWriter::Write("output.obj", mesh.Points, mesh.Triangles);
}
