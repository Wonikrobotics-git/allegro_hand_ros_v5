cmake_minimum_required(VERSION 3.0.2)
project(allegro_hand_octave)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  roslib
)

catkin_package(
  CATKIN_DEPENDS roscpp
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(octave_script_runner src/octave_script_runner.cpp)
target_link_libraries(octave_script_runner ${catkin_LIBRARIES})
