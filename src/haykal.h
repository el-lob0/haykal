#ifndef HAYKAL_H
#define HAYKAL_H

#include <nib.h> 
#include <GLFW/glfw3.h>



#define H_pause_loop_until_event() nib_wait_events()
 

typedef int H_Element;

typedef struct {
    GLFWwindow *window;
    H_Element origin;
    Pixel *main_buffer; 
    int buffer_w;
    int buffer_h;
} H_Window;

typedef struct {
    int w;
    int h;
} H_Size;


#define H_monitor_resize(win, func) nib_read_window_size(win, func)

H_Window H_new_window(const char *title);

void H_update_size( GLFWwindow *window, int w, int h );

void H_wait();

void haykal_init_components(H_Window window, int initial_capacity); 

void H_pause_until_event();

int H_main_loop_running(H_Window window);


int H_show_frame(H_Window *pwindow);


void H_continue_loop();


void H_pause_loop_until_event();


void H_send_fake_event();

typedef int H_Axis;

typedef enum {
  HORIZONTAL,
  VERTICAL,
} Vector;

H_Axis H_create_axis(Vector vec, int master, int offset_x, int offset_y, int sep);

void H_add_to_axis(H_Axis iVec, H_Element iElement);

typedef struct {
  int *top;
  int *bottom;
  int *right;
  int *left;
} Margin;

typedef enum {
  TOP,
  RIGHT,
  BOTTOM,
  LEFT,
  ABSOLUTE
} Anchor;

H_Element H_new_box(int layer, int width, int height, Pixel color, int angle, int radius, int feather, int x, int y, Anchor anchor );


void H_add_child(H_Element iparent, H_Element ichild);


void H_set_children(H_Element iparent, H_Element *ichildren);


#endif 
