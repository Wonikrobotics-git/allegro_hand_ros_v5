//
// Created by felixd on 10/1/15.
//
// Modified by Hibo on 2024/07/10.

#ifndef PROJECT_ALLEGRO_NODE_COMMON_H
#define PROJECT_ALLEGRO_NODE_COMMON_H

// Defines DOF_JOINTS.
#include "allegro_hand_driver/AllegroHandDrv.h"
using namespace allegro;

#include <string>
#include <boost/thread/thread.hpp>

#include <visualization_msgs/Marker.h>
#include <std_msgs/Int32.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

#include "ros/ros.h"
#include "sensor_msgs/JointState.h"
#include "std_msgs/String.h"
#include "std_msgs/Float32.h"
#include "fstream"
#include "bhand/BHand.h"
#include <vector>
// Forward declaration.
class AllegroHandDrv;

// Set control time interval
#define ALLEGRO_CONTROL_TIME_INTERVAL 0.002

// Topic names: current & desired JointState, named grasp to command.
const std::string JOINT_STATE_TOPIC = "allegroHand/joint_states";
const std::string DESIRED_STATE_TOPIC = "allegroHand/joint_cmd"; 
const std::string LIB_CMD_TOPIC = "allegroHand/lib_cmd";  


class AllegroNode {
 public:

  AllegroNode(bool sim = false);

  virtual ~AllegroNode();

  void publishData();

  void desiredStateCallback(const sensor_msgs::JointState &desired);

  void ControltimeCallback(const std_msgs::Float32::ConstPtr& msg);

  void GraspforceCallback(const std_msgs::Float32::ConstPtr& msg);

  virtual void updateController();

  // This is the main method that must be implemented by the various
  // controller nodes.
  virtual void computeDesiredTorque() {
    ROS_ERROR("Called virtual function!");
  };

  ros::Timer startTimerCallback();

  void timerCallback(const ros::TimerEvent &event);

  void Rviz_Arrow();

  double desired_position[DOF_JOINTS] = {0.0};

 protected:

  double current_position[DOF_JOINTS] = {0.0};
  double previous_position[DOF_JOINTS] = {0.0};

  double current_position_filtered[DOF_JOINTS] = {0.0};
  double previous_position_filtered[DOF_JOINTS] = {0.0};

  double current_velocity[DOF_JOINTS] = {0.0};
  double previous_velocity[DOF_JOINTS] = {0.0};
  double current_velocity_filtered[DOF_JOINTS] = {0.0};

  double desired_torque[DOF_JOINTS] = {0.0};

  std::string whichHand;  // Right or left hand.
  std::string whichType;  // Right or left hand.

  // ROS stuff
  ros::NodeHandle nh;
  ros::Publisher  joint_state_pub;
  ros::Publisher  marker_pub;
  ros::Subscriber joint_cmd_sub;
  ros::Subscriber time_sub;
  ros::Subscriber force_sub;

  // Store the current and desired joint states.
  sensor_msgs::JointState current_joint_state;
  sensor_msgs::JointState desired_joint_state;

  // ROS Time
  ros::Time tstart;
  ros::Time tnow;
  double dt;

  // CAN device
  allegro::AllegroHandDrv *canDevice;
  boost::mutex *mutex;

  // Flags
  int lEmergencyStop = 0;
  long frame = 0;

  // RVIZ marker
  visualization_msgs::Marker marker1, marker2, marker3, marker4;

  BHand *pBHand = NULL;

};

double motion_time = 0.5;//
double force_get = 2.0f;//

#endif //PROJECT_ALLEGRO_NODE_COMMON_H
