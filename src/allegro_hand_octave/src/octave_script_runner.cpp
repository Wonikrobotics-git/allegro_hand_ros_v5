#include <cstdlib>
#include <iostream>
#include <ros/package.h>

int main() {
    // allegro_hand 패키지의 경로를 가져옴
    std::string pkg_path = ros::package::getPath("allegro_hand_parameters");
    // allegro_hand 패키지의 경로와 angle/Encoder.m 파일의 경로를 합쳐서 전체 경로 생성
    std::string octaveScriptPath = pkg_path + "/angle/Encoder.m";

    // 옥타브 스크립트를 실행하는 명령어
    system("pkill -f octave-cli");
    
    std::string command = "octave-cli " + octaveScriptPath;

    // 시스템 명령 실행
    system(command.c_str());

    return 0;
}

/*
int main(int argc, char** argv) {
    // ROS 노드 초기화
    ros::init(argc, argv, "matlab_ros_node");
    ros::NodeHandle nh;

    // allegro_hand_parameters 패키지의 경로를 가져옴
    std::string pkg_path = ros::package::getPath("allegro_hand_parameters");
    // 패키지 경로와 angle/Encoder.m 파일의 경로를 합쳐서 전체 경로 생성
    std::string matlabScriptPath = pkg_path + "/angle/Encoder.m";

    // MATLAB 스크립트를 실행하는 명령어
    std::string command = "matlab -batch \"cd('" + pkg_path + "/angle'); run('Encoder.m');\" -softwareopengl";

    // 시스템 명령 실행
    int ret = system(command.c_str());
    if (ret != 0) {
        std::cerr << "Failed to execute MATLAB script. Error code: " << ret << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
*/