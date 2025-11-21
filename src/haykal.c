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
  window.container = -1;
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

// I gotta get better at naming things...
/// This displays the main buffer and [pauses until events ?]
int H_show_frame(H_Window *pwindow) {
  int i = pwindow->container; // index pointing to the main buffer (which is an element as well)

  layout(i);

  nib_display_buffer(pwindow->window, components.buffer[i], components.widths[i], components.heights[i]);
  return 0;
}
