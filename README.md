
# Allegro Hand V5 ROS1

This is the official release to control Allegro Hand V5 with ROS1(Only V5 supported, V4 is not supported). Mostly, it is based on the old release of Allegro Hand V4 ROS1 package.
You can find our old release of Allegro Hand V4 Ros package :(https://github.com/Wonikrobotics-git/allegro_hand_ros_v4)
And the interfaces and controllers have been improved and rewritten by Soohoon Yang(Hibo) from Wonik Robotics.

We reduce several unnecessary package/nodes(allegro_hand,allegro_node_pd...) and modify our main driver code due to changes of the motors.

**Also we now provide some new actions:**
- Save customize pose using Moveit and move to the saved pose.
- Visualize Allegro Hand V5 changing in real time in Rviz.
- Provide encoder and torque graph using Octave.
- Simply control hand with GUI tool instead of using keyboard.

These packages are tested on ROS Melodic(Ubuntu 18.04) and Noetic(Ubuntu 20.04). It will likely not work with previous versions(Kinetic ...).

## Useful Links
- Official Allegro Hand Website : https://www.allegrohand.com/
- Community Forum :  https://www.allegrohand.com/forum

## Packages

**From V5, the hand is fully based on torque controller. So we reduce some packages/nodes related with position controller.**

- allegro_hand_controllers : Contain two main nodes for control the hand.
	- node : Receive encoder data and compute torque using `computeDesiredTorque`.
	- grasp : Apply various pre-defined grasps or customized poses.
- allegro_hand_description : Urdf,xacro descriptions for the kinematics of the hand, rviz configuration and meshes.
- allegro_hand_driver : Main driver for sending and receiving data with the Allegro Hand.
- allegro_hand_gui : Node that control the allegro hand with gui program.
- allegro_hand_keyboard : Node that sends the command to control Allegro Hand. All commands need to be pre-defined.
- allegro_hand_moveit : Provide MOVEIT package for Allegro Hand V5.
- allegro_hand_octave : Provide Octave package for extract graph of encoder data and torque data.
- allegro_hand_parameters : Offset and servo directions for each of the 16 joints, and some meta information about the hand.
- bhand : Library files for the predefined grasps and actions., available on 64 bit versions.

## Install the PCAN driver

**Before using the hand, you must install the pcan drivers. This assumes you have a peak-systems pcan to usb adapter.**

1. Install these packages
- Ubuntu 18.04(melodic)
~~~bash
sudo apt-get install libpopt-dev ros-melodic-libpcan
~~~
- Ubuntu 20.04(noetic)
~~~bash
sudo apt-get install libpopt-dev ros-noetic-libpcan
~~~

2. Download latest drivers: [PEAK_driver](https://www.peak-system.com/fileadmin/media/linux/index.php)
    
- Install the drivers:
~~~bash
make clean; make NET=NO_NETDEV_SUPPORT
sudo make install
sudo modprobe pcan
~~~
- Check if drivers successfully installed:
~~~bash
pcaninfo
~~~
- When the hand is connected, you should see pcanusb0 or pcanusb1 in the list of available interfaces.

## Run main controller nodes

1. Make your own Workspace.
~~~bash
mkdir ~/allegro_ws
~~~

2. Install necessary packages.
~~~bash
sudo apt-get update
sudo apt-get install ros-<distro>-visualization-msgs
sudo apt-get install ros-<distro>-jsk-rviz-plugins
~~~

3. Clone or Download Allegro Hand V5 ROS package.(https://github.com/Wonikrobotics-git/allegro_hand_ros_v5.git)
~~~bash
git clone https://github.com/Wonikrobotics-git/allegro_hand_ros_v5.git
~~~

4. Install BHand library
~~~bash
cd allegro_hand_ros_v5-master-4finger/src/bhand

sudo make install

sudo ldconfig
~~~

5. Build Allegro Hand V5 ROS package.
~~~bash
cd ~/allegro_ws/allegro_hand_ros_v5-master-4finger

catkin_make

source devel/setup.bash
~~~

6. Connect PCAN-USB and Allegro Hand (**make sure to power on Allegro Hand**)
7. Start the ROS package.
~~~bash
source devel/setup.bash
roslaunch allegro_hand_controllers allegro_hand.launch HAND:=right TYPE:=A KEYBOARD:=true
~~~

**If you get 'Permission denied' error, please chmod +x detect_pcan.py in allegro_hand_description/scripts folder**

8. Use keyboard command to move Allegro Hand.

**Please check 'Launch file instructions' below.**

## Launch file instructions

Same as the previous version of the ROS package, you can simply control Allegro Hand V5 by launching `allegro_hand.launch`. It has many arguments, but at a minimum you must specify the handedness and the hand type:
~~~bash
roslaunch allegro_hand_controllers allegro_hand.launch HAND=right|left TYPE:=A|B (A: non-geared, B: geared)
~~~

Optional arguments:
~~~
	VISUALIZE:=true|false (default is false)
	KEYBOARD:=true|false (default is false)
	MOVEIT:=true|false (default is false)
	GUI:=true|false (default is false)
	AUTO_CAN:=true|false (default is false)
~~~

- If you want to visualize Allegro Hand on Rviz:
~~~bash
roslaunch allegro_hand_controllers allegro_hand.launch HAND=right TYPE:=A VISUALIZE:=true
~~~

- If you want to control Allegro Hand with Keyboard:
~~~bash
roslaunch allegro_hand_controllers allegro_hand.launch HAND=right TYPE:=A KEYBOARD:=true
~~~

- If you want to make pose or simulate Allegro Hand with MOVEIT:
 ~~~bash
roslaunch allegro_hand_controllers allegro_hand.launch HAND=right TYPE:=A MOVEIT:=true
~~~

- If you want to control Allegro Hand with Keyboard and GUI:
~~~bash
roslaunch allegro_hand_controllers allegro_hand.launch HAND=right TYPE:=A KEYBOARD:=true GUI:=true
~~~

- **Note on `AUTO_CAN`: There is a nice script `detect_pcan.py` which automatically finds an open `/dev/pcanusb` file. If instead you specify the can device manually (`CAN_DEVICE:=/dev/pcanusbN`), make sure you _also_ specify `AUTO_CAN:=false`. Obviously, automatic detection cannot work with two hands.**

## Control more than one hand

1. Specify the can device:
~~~bash
pcaninfo
~~~

2. Open two terminals and source the allegro hand workspace.

3. Launch Allegro Hand packages using command below.

Terminal 1:
~~~bash
roslaunch allegro_hand_controllers allegro_hand.launch HAND:=right TYPE:=A NUM:=0 CAN_DEVICE:=/dev/pcanusb32 AUTO_CAN:=false KEYBOARD:=true
~~~
Terminal 2:
~~~bash
roslaunch allegro_hand_controllers allegro_hand.launch HAND:=left TYPE:=A NUM:=1 CAN_DEVICE:=/dev/pcanusb33 AUTO_CAN:=false KEYBOARD:=true
~~~

