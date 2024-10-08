#include <ros/ros.h>
#include <jsk_rviz_plugins/OverlayText.h>
#include <std_msgs/ColorRGBA.h>

int main(int argc, char** argv) {
    ros::init(argc, argv, "overlay_text_publisher");
    ros::NodeHandle nh;

    ros::Publisher pub = nh.advertise<jsk_rviz_plugins::OverlayText>("/overlay_text", 1);
    ros::Rate rate(1);  // 1 Hz

    while (ros::ok()) {
        jsk_rviz_plugins::OverlayText text_msg;
        text_msg.width = 600;
        text_msg.height = 100;
        text_msg.left = 10;
        text_msg.top = 10;
        text_msg.text_size = 12;
        text_msg.line_width = 2;
        text_msg.font = "DejaVu Sans Mono";
        text_msg.text = "WONIKROBOTICS";

        std_msgs::ColorRGBA fg_color;
        fg_color.r = 1.0;
        fg_color.g = 1.0;
        fg_color.b = 1.0;
        fg_color.a = 1.0;
        text_msg.fg_color = fg_color;

        std_msgs::ColorRGBA bg_color;
        bg_color.r = 0.0;
        bg_color.g = 0.0;
        bg_color.b = 0.0;
        bg_color.a = 0.5;
        text_msg.bg_color = bg_color;

        pub.publish(text_msg);
        rate.sleep();
    }

    return 0;
}