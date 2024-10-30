#include <ros/ros.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit_msgs/ExecuteTrajectoryAction.h>
#include <actionlib/client/simple_action_client.h>
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <sensor_msgs/JointState.h>
#include <ros/package.h>
#include "std_msgs/String.h"

bool should_save_joint_states = false;

ros::Publisher joint_cmd_pub;


void saveJointStates(const sensor_msgs::JointState::ConstPtr& msg) {
     if (should_save_joint_states) {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "name" << YAML::Value << msg->name;
        out << YAML::Key << "position" << YAML::Value << msg->position;
        out << YAML::EndMap;

        std::string pkg_path = ros::package::getPath("allegro_hand_controllers");
        std::string file_path = pkg_path + "/pose/pose_moveit.yaml";
        std::ofstream fout(file_path);
        fout << out.c_str();
        fout.close();

        std_msgs::String msg;
        std::stringstream ss;

        ss<<"moveit";
        msg.data =ss.str();

        joint_cmd_pub.publish(msg);

        should_save_joint_states = false; 
        ROS_INFO("Joint states save complete.");
     }
}

void executeTrajectoryCallback(const moveit_msgs::ExecuteTrajectoryActionResult::ConstPtr& result) {
    if (result->status.status == actionlib_msgs::GoalStatus::SUCCEEDED) {
        ROS_INFO("Execution succeeded, preparing to save joint states.");
        should_save_joint_states = true; 
    } else {
        ROS_WARN("Execution did not succeed.");
    }
}



int main(int argc, char** argv) {
    ros::init(argc, argv, "save_joint_state");
    ros::AsyncSpinner spinner(1);
    spinner.start();
    ros::NodeHandle nh;

    moveit::planning_interface::MoveGroupInterface move_group("HAND"); //
    joint_cmd_pub = nh.advertise<std_msgs::String>("/allegroHand/lib_cmd", 10);

    actionlib::SimpleActionClient<moveit_msgs::ExecuteTrajectoryAction> execute_trajectory_ac("/execute_trajectory", true);
    execute_trajectory_ac.waitForServer();

    ros::Subscriber execute_trajectory_sub = nh.subscribe("/execute_trajectory/result", 10, executeTrajectoryCallback);
    ros::Subscriber joint_sub = nh.subscribe("/joint_states", 10, saveJointStates);
 
    ros::waitForShutdown();

    return 0;
}