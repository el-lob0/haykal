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
  init_window_bg(window, (Pixel){0.0f, 0.9f, 0.9f, 0.0f});
  haykal_init_components(window, 32);

  window.buffer_h = 500; window.buffer_w = 500;

  window.main_buffer = nib_init_buffer(500, 500);

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



/// To be used as a signal, probably coupled with a flag
void H_send_fake_event() {
  glfwPostEmptyEvent();
}


void H_wait() {
  nib_wait_for_buffer();
}

// BUG: Potentially undefined behaviour
void H_pause_until_event(){
    nib_wait_events();
    H_pause_loop_until_event();
}

void H_update_size( GLFWwindow *window, int w, int h ) {
  
  main.w = w;
  main.h = h;
  H_update_bg_size(w, h);

}

/// This displays the main buffer and [pauses until events ?]
int H_show_frame(H_Window *pWindow) {



  // pWindow->buffer_w = main.w;
  // pWindow->buffer_h = main.h; 
  
  H_draw_main_buffer(*pWindow);

  nib_display_buffer(pWindow->window, main.buffer, main.w, main.h);

  return 0;
}
