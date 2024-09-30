## Main Program

1. Make your own Workspace.
~~~bash
mkdir ~/allegro_ws
~~~

2. Install necessary packages.
~~~bash
sudo apt-get update
sudo apt-get install ros-<distro>-visualization-msgs
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
roslaunch allegro_hand_controllers allegro_hand.launch HAND:=right KEYBOARD:=true
~~~

8. Use keyboard command to move Allegro Hand.
	**NOTE: More information in** https://github.com/Wonikrobotics-git/allegro_hand_ros_v5


## Moveit!

1. Start Moveit
~~~bash
roslaunch allegro_hand_controllers allegro_hand.launch HAND:=right Moveit:=true KEYBOARD:=true
~~~

2. Select joints menu and setup goal position of each joint.
![Screenshot from 2024-06-26 13-54-38|50](https://github.com/user-attachments/assets/a4df8c60-8171-4cc8-90a6-ac2132f7e586)
![Screenshot from 2024-06-26 13-59-29|50](https://github.com/user-attachments/assets/d0b74bfe-05a6-42a2-95ff-7245901e38b8)


3. Select Planning menu and set Start State: `<current>`.

4. Click `Plan`button and `Execute`button.

5. `Execution completed: SUCCEEDED` and `joint states save complete` will be print on terminal.

	**NOTE : After the log is printed, hand will move to goal joint position.**

6. Use Keyboard `spacebar + Num(0~9)` to save current pose.
![[Screenshot from 2024-06-26 14-40-58.png|700]]
	 **NOTE : Pose will be saved with named '(pose + Num). yaml'**.

## Octave

1. Launch main program.
~~~bash
roslaunch allegro_hand_controllers allegro_hand.launch HAND:=right KEYBOARD:=true
~~~

2. Press `s` button on Allegro Hand terminal.
![[Screenshot from 2024-06-26 14-40-58.png|700]]

3. The accumulated data will be plot on your screen.

## GUI

1. Clone or Download Allegro Hand V5 Octave package.(https://github.com/shy0914/HIbo.git)
~~~bash
cd ~/allegro_hand_ros_v5-master-4finger/src
git clone https://github.com/shy0914/HIbo.git

~~~

2. Build.
~~~bash
cd ~/allegro_hand_ros_v5-master-4finger
catkin_make
~~~

3. Launch main program with GUI arguments.
~~~bash
roslaunch allegro_hand_controllers allegro_hand.launch HAND:=right GUI:=true
~~~

4. You can now control the Hand with GUI program.
![[Screenshot from 2024-08-12 10-57-00.png|700]]
#### Button instruction
- Clear log button : Clear log.
- Exit button: Torque off and kill GUI program.
- Change button: Change hand motion time (spin box default: 1, range: 0.8~2.0)
- Apply: Apply force when grasp (default: 2.0, range: 0.0~6.0)
- Vertical bar: Change & Apply real-time grasp force by moving vertical bar.
- Home:  Home pose
- Grasp: 4finger Grap pose
- GravityComp: Gravity Compensate each link.
- Off:  Torqueoff
- Save
	1. Select pose number or type pose fiel name to save.
	2. Save current joint of Moveit in yaml.
- Move
	1. Select Pose number or pose file.
	2. Move Allegro Hand to saved pose.
	3. Refresh : the pose file list will be reloaded.
- Custom Hand Pose Repeat
	1. Select number of poses you want to repeat.
	2. Select poses by paying attention to the order in proportion to the number of poses selected.
	3. Choose how many times to repeat.
	4. Start.
- Reset : Fingertip sensor value reset.
	**NOTE : When saving a pose, the name of pose file must not overlap with predefined pose name.**
