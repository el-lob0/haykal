#include <stdio.h>
#include "../include/glad.c"
// #include "util.c"
#include "setup.c"
#define GLAD_GL_IMPLEMENTATION
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdlib.h>

#include "haykal.h" 
#include <stdio.h>
#include <time.h>



H_Window H_new_window(const char *title) {
  H_Window window;
  window.window = nib_init_os_window(title);
  init_window_bg(window, (Pixel){1.0f, 0.0f, 0.0f, 1.0f});
  haykal_init_components(window, 32);
  return window;
}

/// Returns a bool indicating if the os window is still open. 
int H_main_loop_running(H_Window window) {
  // name changed to make more sense in ui context
  return nib_window_is_open(window.window);
}

void H_continue_loop() {
  nib_poll_events();
}

void H_pause_loop_until_event() {
  nib_wait_events();
}

/// To be used as a signal, probably coupled with a flag
void H_send_fake_event() {
  glfwPostEmptyEvent();
}


void H_wait() {
  nib_wait_for_buffer();
}

void H_update_size( GLFWwindow *window, int w, int h ) {
  H_update_bg_size(w, h);
}


// I gotta get better at naming things...
/// This displays the main buffer and [pauses until events ?]
int H_show_frame(H_Window *pWindow) {

  // need to repplace by a function that returns the main buffer
  H_draw_main_buffer(*pWindow);

  nib_display_buffer(pWindow->window, pWindow->main_buffer, 3000, 3000);
  return 0;
}
