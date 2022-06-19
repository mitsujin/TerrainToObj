set(BINARY ${CMAKE_PROJECT_NAME})

set (SOURCE 
    main.cpp
    height_map.cpp
    mesh_generator.cpp
    obj_writer.cpp
    tri_priority_queue.cpp
)

add_executable(${BINARY} ${SOURCE})
target_link_libraries("${BINARY}")

add_library(${BINARY}_lib STATIC ${SOURCE})

target_compile_features(${BINARY} PRIVATE cxx_std_20)
target_compile_features(${BINARY}_lib PRIVATE cxx_std_20)

