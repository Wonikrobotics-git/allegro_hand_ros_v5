## Moveit!

1. Start Moveit
~~~bash
roslaunch allegro_hand_controllers allegro_hand.launch HAND:=right TYPE:=A MOVEIT:=true KEYBOARD:=true
~~~

2. Select joints menu and setup goal position of each joint.
![Screenshot from 2024-06-26 13-54-38](https://github.com/user-attachments/assets/a4df8c60-8171-4cc8-90a6-ac2132f7e586)
![Screenshot from 2024-06-26 13-59-29](https://github.com/user-attachments/assets/d0b74bfe-05a6-42a2-95ff-7245901e38b8)


3. Select Planning menu and set Start State: `<current>`.

4. Click `Plan`button and `Execute`button.

5. `Execution completed: SUCCEEDED` and `joint states save complete` will be print on terminal.

	**NOTE : After the log is printed, hand will move to goal joint position.**

6. Use Keyboard `spacebar + Num(0~9)` to save current pose.
![Screenshot from 2024-06-26 14-40-58](https://github.com/user-attachments/assets/dc22ac65-98ff-4c38-9077-a2fbe1d4eb79)

	 **NOTE : Pose will be saved with named '(pose + Num). yaml'**.

7. Use Keyboard `Num(0~9)`(same as above num) to move hand to saved pose.

## Octave

1. Launch main program.
~~~bash
roslaunch allegro_hand_controllers allegro_hand.launch HAND:=right TYPE:=A KEYBOARD:=true
~~~

2. Press `s` button on Allegro Hand terminal.
![Screenshot from 2024-06-26 14-40-58](https://github.com/user-attachments/assets/4ba9e2ae-b827-4c89-9f84-87dc7da83663)


3. The accumulated data will be plot on your screen.

## GUI

1. Launch main program with GUI arguments.
~~~bash
roslaunch allegro_hand_controllers allegro_hand.launch HAND:=right TYPE:=A GUI:=true
~~~

2. You can now control the Hand with GUI program.
![Screenshot from 2024-08-12 10-57-00](https://github.com/user-attachments/assets/161a9a1f-9386-4742-80a4-b425b703483f)

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
