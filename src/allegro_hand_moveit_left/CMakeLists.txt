cmake_minimum_required(VERSION 3.1.3)
project(allegro_hand_moveit_left)

find_package(catkin REQUIRED COMPONENTS
  moveit_ros_planning_interface
  roscpp
  rospy
  std_msgs
  sensor_msgs
  robot_state_publisher
    jsk_rviz_plugins
)

catkin_package(
  CATKIN_DEPENDS robot_state_publisher moveit_ros_planning_interface roscpp rospy std_msgs sensor_msgs
)

include_directories(
  ${catkin_INCLUDE_DIRS}
    ${YAML_CPP_INCLUDE_DIRS}
)

add_executable(save_joint_angle src/save_joint_angles.cpp)
target_link_libraries(save_joint_angle ${catkin_LIBRARIES} yaml-cpp)

add_executable(overlay_text_publishers src/text_publisher.cpp)
target_link_libraries(overlay_text_publishers
  ${catkin_LIBRARIES}
)

add_executable(allegro_joint_subscribers src/allegro_joint_subscriber.cpp)
target_link_libraries(allegro_joint_subscribers
  ${catkin_LIBRARIES}
)

install(DIRECTORY launch DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
  PATTERN "setup_assistant.launch" EXCLUDE)
install(DIRECTORY config DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
