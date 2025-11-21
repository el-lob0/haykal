#include "../include/nib.h"
#include <GLFW/glfw3.h>
#define PixelBuffer Pixel*



typedef struct {
  GLFWwindow *window;
  int container;  // an index in H_Elements, poiting to the box 
  // inside which the window's layout will be layed out.
} H_Window;

typedef enum {
  HORIZONTAL,
  VERTICAL,
  ABSOLUTE,
} Orientation ;

// i will identify the widgets by their index 
typedef struct {
  Pixel **buffer;
  int *widths;
  int *heights;
  Orientation mode;
  int padding;
  int margin;
} H_Components ;
// NOTE: I need to malloc at every new addition to the arrays.

// for clarity, this is the index of the element, which gets passed as the element itself
typedef int H_Element; 


H_Window H_new_window(const char *title) {
  H_Window window;
  window.window = nib_init_os_window(title);
  window.container = -1;
  return window;
}

void H_set_window_child(H_Window *pwindow, int container) {
  pwindow->container = container;
}

/// Returns a bool indicating if the os window is still open. 
int H_run_main_loop(H_Window window) {
  // I abstracted it on purpose
  return nib_window_is_open(window.window);
}

/// I gotta get better at naming things...
/// This displays the main buffer and [pauses until events ?]
int H_show_frame(H_Window *pwindow, H_Components element_list /* if passing the whole thing is too much i have to find another way */) {
  int i = pwindow->container; // index pointing to the main buffer
  nib_display_buffer(pwindow->window, element_list.buffer[i], element_list.widths[i], element_list.heights[i]);
  return 0;
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

/// PRIVATE FUNCTION | This is supposed to draw the final window buffer to be shown
void H_draw_main_buffer(Pixel *bg_buffer, Pixel **fg_buffers, H_Window window) {
  int len = sizeof(**fg_buffers)/sizeof(Pixel*);
  int i;
  for (i=0; i<len; i++) {
    // need to exact the layout system before i get into this
  }
}
















