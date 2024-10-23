#include <ros/ros.h>
#include "std_msgs/String.h"

#include <iostream>
#include <signal.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include "virtualkey_codes.h"
#include "allegro_hand_driver/AllegroHandDrv.h"
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <vector>
#include <ros/package.h>
#include "virtualkey_codes.h"

using namespace std;

#define DOF_JOINTS 16


class AHKeyboard
{
public:
  AHKeyboard();
  void keyLoop();
  void printUsage();

private:
  ros::NodeHandle nh_;
  ros::Publisher cmd_pub_;

  void savePose(const std::string& pose_file);
  std::vector<double> readFinalJointStates();
};

AHKeyboard::AHKeyboard()
{
  cmd_pub_ = nh_.advertise<std_msgs::String>("allegroHand/lib_cmd", 10);
}


int kfd = 0;
struct termios cooked, raw;

void quit(int sig)
{
  tcsetattr(kfd, TCSANOW, &cooked);
  ros::shutdown();
  exit(0);
}

std::vector<double> AHKeyboard::readFinalJointStates()
{
  // 입력 받은 숫자에 해당하는 pose 파일 경로 생성
  std::string pkg_path = ros::package::getPath("allegro_hand_controllers");
  std::string file_path = pkg_path + "/pose/pose_moveit.yaml";

    // pose 파일 읽기
  YAML::Node node = YAML::LoadFile(file_path);
  std::vector<double> positions = node["position"].as<std::vector<double>>();
  return positions;
}

void AHKeyboard::savePose(const std::string& pose_file)
{
  std::vector<double> positions = readFinalJointStates();

  YAML::Emitter out;
  out << YAML::BeginMap;
  out << YAML::Key << "position" << YAML::Value << positions;
  out << YAML::EndMap;

  std::string pkg_path = ros::package::getPath("allegro_hand_controllers");
  std::string file_path = pkg_path + "/pose/" + pose_file;

 // ROS_INFO("Package path: %s", pkg_path.c_str());
 // ROS_INFO("Full file path: %s", file_path.c_str());

  std::ofstream fout(file_path);
  fout << out.c_str();
  fout.close();
  ROS_INFO("Pose saved to %s", pose_file.c_str());
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "allegro_hand_keyboard");
  AHKeyboard allegro_hand_keyboard_cmd;

  signal(SIGINT,quit);

  allegro_hand_keyboard_cmd.keyLoop();

  return(0);
}

void AHKeyboard::printUsage() {
  std::cout << std::endl;
  std::cout << " -----------------------------------------------------------------------------" << std::endl;
  std::cout << "  Use the keyboard to send Allegro Hand grasp & motion commands" << std::endl;
  std::cout << " -----------------------------------------------------------------------------" << std::endl;

  std::cout << "\tHome Pose:\t\t\t'H'" << std::endl;
  std::cout << "\tPinch (index+thumb):\t\t'P'" << std::endl;
  std::cout << "\tPinch (middle+thumb):\t\t'M'" << std::endl;
  std::cout << "\tGrasp (3 fingers):\t\t'G'" << std::endl;
  std::cout << "\tGrasp (4 fingers):\t\t'K'" << std::endl;
  std::cout << "\tGrasp (envelop):\t\t'E'" << std::endl;
  std::cout << "\tGravity compensation:\t\t'A'" << std::endl;
  std::cout << "\tMotors Off (free motion):\t'F'" << std::endl;

  std::cout << " -----------------------------------------------------------------------------" << std::endl;
  std::cout << "  MOVE IT\t(Need to install moveit package)" << std::endl;
  std::cout << " -----------------------------------------------------------------------------" << std::endl;
  std::cout << "\tPD Control (Custom Pose) :\t'0 ~ 9'" << std::endl;
  std::cout << "\tSave Latest Moveit Pose:\t'Space + 0 ~ 9'" << std::endl;

  std::cout << " -----------------------------------------------------------------------------" << std::endl;
  std::cout << "  OCTAVE\t(Need to install Octave)" << std::endl;
  std::cout << " -----------------------------------------------------------------------------" << std::endl;
  std::cout << "\tSave Data & Plot:\t\t'S'" << std::endl;
  std::cout << " -----------------------------------------------------------------------------" << std::endl;
  std::cout << "  Subscriber code for reading these messages is included in '~core_template'." << std::endl;
  std::cout << " -----------------------------------------------------------------------------\n" << std::endl;

}

#define HANDLE_KEYCODE(keycode, pose_num) \
  case keycode: \
    if (!space_pressed) { \
      ss << "pdControl" << pose_num; \
      dirty = true; \
    } else { \
      ROS_DEBUG(#keycode "_key: Save Pose " #pose_num); \
      savePose("pose" #pose_num ".yaml"); \
    } \
    break;

void AHKeyboard::keyLoop()
{
  char c;
  bool dirty=false;
  bool space_pressed = false;

  // get the console in raw mode
  tcgetattr(kfd, &cooked);
  memcpy(&raw, &cooked, sizeof(struct termios));
  raw.c_lflag &=~ (ICANON | ECHO);
  // Setting a new line, then end of file
  raw.c_cc[VEOL] = 1;
  raw.c_cc[VEOF] = 2;
  tcsetattr(kfd, TCSANOW, &raw);

  sleep(2);
  printUsage();

  for(;;)
  {
    std_msgs::String msg;
    std::stringstream ss;

    // get the next event from the keyboard
    if(read(kfd, &c, 1) < 0)
    {
      perror("read():");
      exit(-1);
    }

    ROS_DEBUG("value: 0x%02X\n", c);
    switch(c)
    {

      HANDLE_KEYCODE(KEYCODE_0, 0)
      HANDLE_KEYCODE(KEYCODE_1, 1)
      HANDLE_KEYCODE(KEYCODE_2, 2)
      HANDLE_KEYCODE(KEYCODE_3, 3)
      HANDLE_KEYCODE(KEYCODE_4, 4)
      HANDLE_KEYCODE(KEYCODE_5, 5)
      HANDLE_KEYCODE(KEYCODE_6, 6)
      HANDLE_KEYCODE(KEYCODE_7, 7)
      HANDLE_KEYCODE(KEYCODE_8, 8)
      HANDLE_KEYCODE(KEYCODE_9, 9)

      case VK_SPACE:
        space_pressed = true;
        break;

      case KEYCODE_h:
        ROS_DEBUG("h_key: Home");
        OperatingMode = 0;
        ss << "home";
        dirty = true;
        break; 

      case KEYCODE_s:
        ROS_DEBUG("s_key: save");
        OperatingMode = 0;
        ss << "save";
        dirty = true;
      break;

      case KEYCODE_k:
        ROS_DEBUG("g_key: Grasp (4 finger)");
        OperatingMode = 0;
        ss << "grasp_4";
        dirty = true;
        break;

      case KEYCODE_g:
        ROS_DEBUG("g_key: Grasp (3 finger)");
        OperatingMode = 0;
        ss << "grasp_3";
        dirty = true;
        break;

      case KEYCODE_p:
        ROS_DEBUG("p_key: Pinch (index)");
        OperatingMode = 0;
        ss << "pinch_it";
        dirty = true;
        break;

      case KEYCODE_m:
        ROS_DEBUG("m_key: Pinch (middle)");
        OperatingMode = 0;
        ss << "pinch_mt";
        dirty = true;
        break;

      case KEYCODE_e:
        ROS_DEBUG("e_key: Envelop");
        OperatingMode = 0;
        ss << "envelop";
        dirty = true;
        break;

      case KEYCODE_a:
        ROS_DEBUG("a_key: Gravcomp");
        OperatingMode = 0;
        ss << "gravcomp";
        dirty = true;
        break;

      case KEYCODE_f:
        ROS_DEBUG("f_key: Servos Off");
        OperatingMode = 0;
        ss << "off";
        dirty = true;
        break;
        
      case KEYCODE_slash:
      case KEYCORD_question:
        printUsage();
        break;
    }

    if(c >= KEYCODE_0 && c <= KEYCODE_9) {
      space_pressed = false;
    } else if (c == VK_SPACE) {
      space_pressed = true;
    }

    if(dirty ==true)
    {
      msg.data = ss.str();
      cmd_pub_.publish(msg);
      ros::spinOnce();
      dirty=false;
    }
  }

  return;
}
