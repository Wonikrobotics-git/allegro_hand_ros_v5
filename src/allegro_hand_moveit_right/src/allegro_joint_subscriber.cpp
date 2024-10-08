#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <std_msgs/Empty.h>
#include <vector>
#include <thread>

std::vector<double> allegro_joint(16, 0.0);

void callback(const sensor_msgs::JointState::ConstPtr& msg) {
    for (int i = 0; i < 16; ++i) {
        allegro_joint[i] = msg->position[i];
    }
}

void spinThread() {
    ros::spin();
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "allegro_joint_subscriber");
    ros::NodeHandle nh;

    ros::Publisher pub = nh.advertise<sensor_msgs::JointState>("/joint_states", 10);
    ros::Subscriber sub = nh.subscribe("/allegroHand_0/joint_states", 3, callback);
    ros::Publisher rviz_moveit_update = nh.advertise<std_msgs::Empty>("/rviz/moveit/update_start_state", 1);

    // Spin in a separate thread
    std::thread spinnerThread(spinThread);

    ros::Rate rate(1000); //

    while (ros::ok()) {
        sensor_msgs::JointState joint_state;

        joint_state.header.stamp = ros::Time::now();
        joint_state.name = {"joint_0_0", "joint_1_0", "joint_2_0", "joint_3_0",
                            "joint_4_0", "joint_5_0", "joint_6_0", "joint_7_0",
                            "joint_8_0", "joint_9_0", "joint_10_0", "joint_11_0",
                            "joint_12_0", "joint_13_0", "joint_14_0", "joint_15_0"};

        joint_state.position = {allegro_joint[0], allegro_joint[1], allegro_joint[2], allegro_joint[3],
                                allegro_joint[4], allegro_joint[5], allegro_joint[6], allegro_joint[7],
                                allegro_joint[8], allegro_joint[9], allegro_joint[10], allegro_joint[11],
                                allegro_joint[12], allegro_joint[13], allegro_joint[14], allegro_joint[15]};

        joint_state.velocity.clear();
        joint_state.effort.clear();

        // Publish the joint state to update the current joint values in MoveIt
        pub.publish(joint_state);

        // Notify RViz and MoveIt to update the start state
        std_msgs::Empty empty_msg;
        rviz_moveit_update.publish(empty_msg);

        rate.sleep();
    }

    spinnerThread.join();
    return 0;
}