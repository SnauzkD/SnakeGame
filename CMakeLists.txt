# CMakeList.txt : CMake project for SnakeGame, include source and define
# project specific logic here.
set(SDL3_image_DIR "D:/SDL3Install/SDL3_image-devel-3.1.0-VC/SDL3_image-3.1.0/cmake")
set(SDL3_mixer_DIR "D:/SDL3Install/SDL3_mixer-devel-3.2.0-VC/SDL3_mixer-3.2.0/cmake")
find_package(SDL3_image REQUIRED)
find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_mixer REQUIRED)
#Add source to this project's executable.
add_executable (SnakeGame "SnakeGame.cpp" "SnakeGame.h" "GameObject.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SnakeGame PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(SnakeGame PRIVATE 
SDL3::SDL3
SDL3_ttf::SDL3_ttf
SDL3_image::SDL3_image
SDL3_mixer::SDL3_mixer
)
# TODO: Add tests and install targets if needed.
