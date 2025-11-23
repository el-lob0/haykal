#include <GLFW/glfw3.h>
#include "haykal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"



/* 

 iname = index of name
 pname = pointer to name

 */

typedef struct {
  int **layers;
} H_Layers ;



// widget id is a fixed index 
typedef struct {
  /// widget's buffer is there for direct modification (1D array)
  Pixel **buffer;
  int *widths;
  int *heights;
  int *radius;
  int *angle;
  int *feather;
  int *layer;
  Pixel *color;
} H_Metadata ;
// NOTE: I need to malloc at every new addition to the arrays ehehehehe... (man, i miss rust..)

// internal runtime variable so i init it here
static H_Metadata components = { 0 };

void haykal_init_components(int initial_capacity) {
    arrsetcap(components.buffer, initial_capacity);
    arrsetcap(components.widths, initial_capacity);
    arrsetcap(components.heights, initial_capacity);
    arrsetcap(components.radius, initial_capacity);
    arrsetcap(components.feather, initial_capacity);
    arrsetcap(components.angle, initial_capacity);
    arrsetcap(components.layer, initial_capacity);
    arrsetcap(components.color, initial_capacity);
}

void init_window_bg(H_Window window, Pixel color) {
  arrpush(components.color, color);
  arrpush(components.layer, 0);
  arrpush(components.buffer, nib_rectangle((Pixel){0.5f, 0.5f, 0.5f, 1.0f}, window.buffer_h, window.buffer_w));
  arrpush(components.widths, 1);
  arrpush(components.heights, 1);
  arrpush(components.radius, 0);
  arrpush(components.angle, 0);
  arrpush(components.feather, 0);
}

/// The window's layer number 0, its size depends on the window's size
/// Defaults to gray
void H_update_window_background(H_Window window, Pixel color) {
  components.color[0] = color;
  components.buffer[0] = nib_rectangle(color, window.buffer_h, window.buffer_w);
}









H_Element H_new_box(int width, int height, Pixel color, Orientation orientation, int radius, int feather, int padding[4] ) {

  H_Element ibox;

  ibox = arrlen(components.heights);

  arrpush(components.heights, height);

  printf("%d", ibox); fflush(stdout);
  arrpush(components.buffer, nib_rectangle(color, width, height));
  arrpush(components.color, color);
  arrpush(components.radius, radius);
  arrpush(components.feather, feather);
  arrpush(components.widths, width);

  arrsetcap(components.buffer[ibox], width*height);


  return ibox;
}


void H_set_window_child(H_Window *pwindow, H_Element iOrigin) {
  pwindow->origin = iOrigin;
}


