#include <stdio.h>
#include <webots/camera.h>
#include <webots/camera_recognition_object.h>
#include <webots/motor.h>
#include <webots/robot.h>
#include <math.h>


#define SPEED 1.5
#define TIME_STEP 64

int main() {
  WbDeviceTag camera, left_motor, right_motor;
  wb_robot_init();
  int k = 0;
  double x1=0;
  double y1=0;
  double x2=0;
  double y2=0;
  double x3=0;
  double y3=0;


  /* Get the camera device, enable it and the recognition */
  camera = wb_robot_get_device("camera");
  wb_camera_enable(camera, TIME_STEP);
  wb_camera_recognition_enable(camera, TIME_STEP);

  
  /* get a handler to the motors and set target position to infinity (speed control). */
  left_motor = wb_robot_get_device("left wheel motor");
  right_motor = wb_robot_get_device("right wheel motor");
  wb_motor_set_position(left_motor, INFINITY);
  wb_motor_set_position(right_motor, INFINITY);



  /* Main loop */
  while (wb_robot_step(TIME_STEP) != -1) 
  {
    const WbCameraRecognitionObject *objects = wb_camera_recognition_get_objects(camera);
    int num = wb_camera_recognition_get_number_of_objects(camera);
    if (num == 0)
    {
       wb_motor_set_velocity(left_motor, -SPEED);
       wb_motor_set_velocity(right_motor, SPEED);
       k=0;
    }
    /* Get and display all the objects information */
    if (num > 0 && k<1)
    {
      x1= objects[0].position[0];
      y1= objects[0].position[1];
      x2= objects[1].position[0];
      y2= objects[1].position[1];
      x3= objects[2].position[0];
      y3= objects[2].position[1];
      printf("Coordinates of player 1: %lf, %lf  \n", x1, y1);
      printf("Coordinates of player 2: %lf,  %lf \n", x2, y2);
      printf("Coordinates of player 3: %lf,  %lf \n", x3, y3);
      if (x1>=x2 && x1>=x3)
      {
       printf("Player 1 is goal keeper \n");
       printf("Player 2 & 3 are defender \n");
      }
      else if (x2>=x1 && x2>=x3)
      {
       printf("Player 2 is goal keeper \n");
       printf("Player 1 & 3 are defender \n");
      }
      else
      {
       printf("Player 3 is goal keeper \n");
       printf("Player 1 & 2 are defender \n");
      }
     k++;
   }
    if (objects[0].position_on_image[0] < 110)
    {
    wb_motor_set_velocity(left_motor, -SPEED);
    wb_motor_set_velocity(right_motor, SPEED);
    }
    else if (objects[0].position[0] > 4.5)
    {
    wb_motor_set_velocity(left_motor, SPEED);
    wb_motor_set_velocity(right_motor, SPEED);
    }
    else
    {
    wb_motor_set_velocity(left_motor, 0.0);
    wb_motor_set_velocity(right_motor, 0.0);
    printf("Forward role reached \n");
    break;
    }

 } 

  wb_robot_cleanup();

  return 0;
}
