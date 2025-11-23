#ifndef HAYKAL_H
#define HAYKAL_H

#include <nib.h> 
#include <GLFW/glfw3.h>


 

typedef int H_Element;

typedef enum {
    HORIZONTAL,
    VERTICAL,
} Orientation ;

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



H_Window H_new_window(const char *title);



void H_wait();



void H_set_window_child(H_Window *pwindow, H_Element container);


int H_main_loop_running(H_Window window);


int H_show_frame(H_Window *pwindow);


void H_continue_loop();


void H_pause_loop_until_event();


void H_send_fake_event();



H_Element H_new_box(int width, int height, Pixel color, Orientation orientation, int radius, int feather, int padding[4]);


void H_add_child(H_Element iparent, H_Element ichild);


void H_set_children(H_Element iparent, H_Element *ichildren);


#endif 
