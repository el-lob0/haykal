// #include "../include/haykal.h"
// #include "../../../include/nib.h"
#define GLAD_GL_IMPLEMENTATION
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <haykal.h>
#include <stdio.h>


int main() {

  H_Window window = H_new_window("LeBaron James");

  int padding[4] = {0,0,0,0};
  H_Element box = H_new_box(200, 500, (Pixel){0.0f, 1.0f, 0.8f, 0.0f}, VERTICAL, 1, 1, padding);
  H_Element top = H_new_box(50, 50, (Pixel){0.3f, 1.0f, 0.8f, 0.0f}, VERTICAL, 5, 1, padding);
  H_Element bottom = H_new_box(50, 50, (Pixel){0.9f, 0.2f, 0.4f, 0.0f}, VERTICAL, 5, 1, padding);

  H_set_window_child(&window, box);

  H_add_child(box, top);
  H_add_child(box, bottom);



  while (H_main_loop_running(window)) {

    H_show_frame(&window);

    H_continue_loop(); 
  }

  return 0;
}

























