// #include "../include/haykal.h"
// #include "../../../include/nib.h"
#define GLAD_GL_IMPLEMENTATION
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <haykal.h>
#include <stdio.h>


int main() {

  H_Window window = H_new_window("LeBaron James");

  H_Element box = H_new_box(1, 300, 300, (Pixel){1.0f, 0.0f, 0.0f, 1.0f}, 0, 20, 1, 5, 5);

  H_monitor_resize(window.window, H_update_size);

  

  while (H_main_loop_running(window)) {

    H_show_frame(&window);

    nib_wait_events();
    H_pause_loop_until_event(); 

    // glfwWaitEventsTimeout(20);

  }

  return 0;
}

























