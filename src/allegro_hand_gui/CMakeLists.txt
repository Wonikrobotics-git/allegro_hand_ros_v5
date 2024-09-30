cmake_minimum_required(VERSION 3.0.2)
project(allegro_hand_gui)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp roslib)

# Qt5 components
find_package(Qt5Widgets REQUIRED)

# Include headers
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${Qt5Widgets_INCLUDE_DIRS}
   ${YAML_CPP_INCLUDE_DIRS}
)

# Declare a catkin package
catkin_package(
  CATKIN_DEPENDS roscpp std_msgs
)

# Source files
set(SOURCES
  src/main.cpp
  src/mainwindow.cpp
)

# UI files
set(UIS
  ui/mainwindow.ui
)

# Qt5 setup
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Ensure CMake knows about the UI files directory
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Add executable
add_executable(${PROJECT_NAME}_node ${SOURCES} ${UIS})

# Link against Qt5 and ROS libraries
target_link_libraries(${PROJECT_NAME}_node
  ${catkin_LIBRARIES}
  Qt5::Widgets yaml-cpp 
)
