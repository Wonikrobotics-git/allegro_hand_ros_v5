#include <cstdlib>
#include <iostream>
#include <ros/package.h>

int main() {

    std::string pkg_path = ros::package::getPath("allegro_hand_parameters");

    std::string octaveScriptPath = pkg_path + "/angle/Encoder.m";


    system("pkill -f octave-cli");
    
    std::string command = "octave-cli " + octaveScriptPath;


    system(command.c_str());

    return 0;
}