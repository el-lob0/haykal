// #include "../include/haykal.h"
// #include "../../../include/nib.h"
#define GLAD_GL_IMPLEMENTATION
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <haykal.h>
#include <stdio.h>


int main() {

  H_Window window = H_new_window("LeBaron James");

  H_Element box1 = H_new_box(1, 900, 100, (Pixel){1.0f, 0.0f, 0.0f, 1.0f}, 0 , 10 , 1, 0, 0, TOP);


  H_Element box2 = H_new_box(2, 100, 100, (Pixel){0.3f, 0.2f, 0.7f, 1.0f}, 0, 0 , 1, 0, 0, ABSOLUTE);


  H_Element box3 = H_new_box(2, 100, 100, (Pixel){1.0f, 0.5f, 1.0f, 1.0f}, 0 , 0, 1, 0, 0, ABSOLUTE);


  H_Element box4 = H_new_box(2, 70 , 70 , (Pixel){1.0f, 1.0f, 0.0f, 1.0f}, 0  , 0, 1, 0, 0, ABSOLUTE);


  H_Element box5 = H_new_box(2, 100, 100, (Pixel){1.0f, 0.0f, 1.0f, 1.0f}, 0 , 0, 1, 400, 400, ABSOLUTE);



  H_add_margin(box1, 5, 0, 0, 0); 
 

  H_Axis axis_the_first = H_create_axis(HORIZONTAL, box1, 0, 0, 50);


  H_add_to_axis(axis_the_first, box2);
  H_add_to_axis(axis_the_first, box3);
  H_add_to_axis(axis_the_first, box4);
  H_add_to_axis(axis_the_first, box5);



  H_monitor_resize(window.window, H_update_size);
  

  while (H_main_loop_running(window)) {

    H_show_frame(&window);

    H_pause_until_event();

  }

  return 0;
}

























