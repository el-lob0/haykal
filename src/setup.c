#include "../include/nib.h"
#include <GLFW/glfw3.h>


/* 

 iname = index of name
 pname = pointer to name

 */


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

typedef int H_Element; // index of element 

typedef struct {
    int w;
    int h;
} H_Size;


// i will identify the widgets by their index 
typedef struct {
  Pixel **buffer;
  H_Element **children;
  int **padding; // array of arrays of paddings, in this order: [left, top, right, bottom] (aka clockwise)
  int *widths;
  int *heights;
  Orientation *mode;
  int *taken_width;
  int *taken_height;
  H_Element *parents;  // index pointing to a Component's parent
  int *child_count;
  int *radius;
  int *feather;
} H_Components ;
// NOTE: I need to malloc at every new addition to the arrays ehehehehe... (man, i miss rust..)

// internal runtime variable so i init it here
static H_Components components = {0};



/*   
 
 extract child's requested size.
 resolve container's size.
 build a buffer with the container's size, and child inside with transparent pixels around
 merge this new buffer with the parent buffer 
 keep doing it...

 Its a tree logic.
 Turns out doing recursive binary trees with ocaml in uni wasnt useless afterall..

 */


void layout(H_Element inode) { 

  // case where its a leaf
  if (components.child_count[inode] == 0) {
    int width = components.widths[inode];
    int height = components.heights[inode];
    
    int iparent = components.parents[inode];
    
    int parent_w = components.widths[iparent];
    int parent_h = components.heights[iparent];

    Pixel *child_buffer = nib_init_buffer(parent_w, parent_h);

    if (components.mode[iparent] == VERTICAL) {
      
      int taken = components.taken_height[iparent]; // x number of rows from the first row 
      int col; int row;

      for (row=taken; row<parent_h; row++) {
        for (col=0; col<parent_w; col++) {

          int ipx_parent = (row * parent_w) + col;
          int ipx_node = (row-taken)*parent_w + col;
          child_buffer[ipx_parent] = child_buffer[ipx_node];
        }
      }
    } else {
      int taken = components.taken_width[iparent]; // x number of rows from the first row 
      int col; int row; int n = 0;
      
      for (row=0; row<parent_h; row++) {
        for (col=taken; col<parent_w; col++) {

          int ipx_parent = (row * parent_w) + col;
          int ipx_node = (row)*parent_w + col-taken;
          child_buffer[ipx_parent] = child_buffer[ipx_node];
        }
        n++;
      }
    }
    components.taken_height[iparent] += height;
    components.taken_width[iparent] += width;

    // and then i do the merging here


  } else {
    int *children = components.children[inode];
    int i;
    for (i=0; i<components.child_count[inode]; i++) {
      layout(components.children[inode][i]);
    }
  }
}

/*
 
 I need a way to 

 */












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
















