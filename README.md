# Robotic Arm Motion Control

For this project I decided to enhance a Robotics Arm Kit I had when I was a kid. This robotic arm can be controlled by the movement of your hand. Using an accelerometer-gyroscope module, hand movements can be mapped to certain movements of the robotic arm. To create the robotic arm control module, I had to rebuild the controller circuitry (used in the wired controller the kit originally came with). I reverse engineered the switch logic of the original controller by implementing motor control (H-Bridge) modules with a few transistors for the switches. An Arduino Uno microcontroller was used to program the robot. The robot responds to movement of the glove module. Some controls include lifting your hand will lift the upper subsection of the robot, twisting your hand will rotate the base of the robot, and letting go of the force sensitive resistor will release the robot’s gripper.



https://user-images.githubusercontent.com/47327154/154851498-1b74f2ee-d5ec-45d5-8476-7b1e82ee22a0.mp4

![IMG_8972](https://user-images.githubusercontent.com/47327154/154851510-d43f35f1-3c13-45e9-85d1-70c97583cf49.JPG)
