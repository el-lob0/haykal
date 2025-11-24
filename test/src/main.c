// #include "../include/haykal.h"
// #include "../../../include/nib.h"
#define GLAD_GL_IMPLEMENTATION
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <haykal.h>
#include <stdio.h>


int main() {

  H_Window window = H_new_window("LeBaron James");

  H_Element box = H_new_box(1, 300, 300, (Pixel){1.0f, 0.0f, 0.0f, 1.0f}, 0, 40, 1, 100, 100);


  H_Element box2 = H_new_box(2, 200, 200, (Pixel){0.3f, 0.2f, 0.7f, 1.0f}, 0, 30, 1, 150, 150);


  H_Element box3 = H_new_box(1, 100, 400, (Pixel){0.0f, 0.0f, 1.0f, 1.0f}, 20, 20, 1, 20, 20);


  H_Element box4 = H_new_box(1, 70, 70, (Pixel){1.0f, 1.0f, 0.0f, 1.0f}, 0, 0, 10, 500, 100);


  H_Element box5 = H_new_box(1, 100, 100, (Pixel){1.0f, 0.0f, 1.0f, 1.0f}, 0, 50, 4, 100, 450);


  H_monitor_resize(window.window, H_update_size);

  

  while (H_main_loop_running(window)) {

    H_show_frame(&window);

    H_pause_until_event();

  }

  return 0;
}

























