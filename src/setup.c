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

static H_Layers layers = {0};

// widget id is a fixed index 
typedef struct {
  /// widget's buffer is there for direct modification (1D array)
  Pixel **buffer;
  int *widths;
  int *heights;
  int *radius;
  int *angle;
  int *feather;
  int *position_x;
  int *position_y;
  int *layer;
  int *visibilty;
  Pixel *color;
} H_Metadata ;

// internal runtime variable so i init it here
static H_Metadata components = { 0 };

void haykal_init_components(H_Window window, int initial_capacity) {
    // arrsetcap(window.main_buffer, initial_capacity);
    arrsetcap(components.buffer, initial_capacity);
    arrsetcap(components.widths, initial_capacity);
    arrsetcap(components.heights, initial_capacity);
    arrsetcap(components.radius, initial_capacity);
    arrsetcap(components.feather, initial_capacity);
    arrsetcap(components.position_x, initial_capacity);
    arrsetcap(components.position_y, initial_capacity);
    arrsetcap(components.angle, initial_capacity);
    arrsetcap(components.layer, initial_capacity);
    arrsetcap(components.color, initial_capacity);
    arrsetcap(layers.layers, initial_capacity);
}

void H_update_layers(int new_layer, H_Element index) {
  if (new_layer > arrlen(layers.layers)) {
    int layer_add = new_layer - arrlen(layers.layers); int i;

    for (i=0; i<layer_add; i++) {
      int *layer = NULL;
      arrpush(layers.layers, layer);
    }
  }
}

void init_window_bg(H_Window window, Pixel color) {
  arrpush(components.color, color);
  arrpush(components.layer, 0);
  arrpush(components.buffer, nib_rectangle((Pixel){1.0f, 1.0f, 1.0f, 1.0f}, 3000, 3000));
  arrpush(components.widths, 1);
  arrpush(components.heights, 1);
  arrpush(components.radius, 0);
  arrpush(components.angle, 0);
  arrpush(components.position_x, 0);
  arrpush(components.position_y, 0);
  arrpush(components.visibilty, 1);
  arrpush(components.feather, 0);
  H_update_layers(0, 0);
}

void push_metadata(int layer, int width, int height, Pixel color, int angle, int radius, int feather, int x, int y, int is_visible ) {
  arrpush(components.color, color);
  arrpush(components.layer, layer);
  arrpush(components.buffer, nib_init_buffer(width, height));
  arrpush(components.widths, width);
  arrpush(components.heights, height);
  arrpush(components.radius, radius);
  arrpush(components.angle, angle);
  arrpush(components.position_x, x);
  arrpush(components.position_y, y);
  arrpush(components.feather, feather);
  arrpush(components.visibilty, is_visible);
}

/// The window's layer number 0, its size depends on the window's size
/// Defaults to gray
void H_update_window_background(Pixel color) {
  components.color[0] = color;
}

void H_update_bg_size(int w, int h) {
  components.widths[0] = w;
  components.heights[0] = h;
  components.buffer[0] = nib_rectangle(components.color[0], w, h);
}

/// New box is assigned to a layer, given a position and a size.
/// Visibility defaults to 1
H_Element H_new_box(int layer, int width, int height, Pixel color, int angle, int radius, int feather, int x, int y ) {

  H_Element ibox;
  ibox = arrlen(components.layer);

  push_metadata(layer, width, height, color, angle, radius, feather, x, y, 1);

  Pixel *tmp_buffer = nib_rectangle(color, width, height);

  // rotation needs padding to avoid clipping
  if (angle > 0) {
    int w = width*2; int h = height*2;
    nib_add_padding(tmp_buffer, width, height, height/2, width/2, height/2, width/2, (Pixel){0.f, 0.f, 0.f, 0.f}, &w, &h);
  }
  nib_apply_radius(tmp_buffer, width, height, radius);
  nib_rotate(tmp_buffer, angle, width, height);
  nib_apply_antialiasing(tmp_buffer, width, height, feather);

  return ibox;
}

void H_set_position(H_Element iElement, int x, int y) {
  components.position_x[iElement] = x;
  components.position_y[iElement] = y;
}

void H_get_position(H_Element iElement, int *x, int *y) {
  x = &components.position_x[iElement];
  y = &components.position_y[iElement];
}

void H_set_dimensions(H_Element iElement, int w, int h) {
  components.heights[iElement] = w;
  components.widths[iElement] = h;
  // FIXME: this needs to modify the actual buffer as well
}

void H_get_dimentions(H_Element iElement, int *w, int *h) {
  w = &components.widths[iElement];
  h = &components.heights[iElement];
}

void H_set_angle(H_Element iElement, int angle) {
  components.angle[iElement] = angle;
  // FIXME: this needs to modify the actual buffer as well
}

void H_get_angle(H_Element iElement, int *angle) {
  angle = &components.angle[iElement];
}

void H_set_visibility(H_Element iElement, int viz) {
  components.visibilty[iElement] = viz;
}

void H_get_visibility(H_Element iElement, int *viz) {
  viz = &components.visibilty[iElement];
}


// TODO: check for overlapping buffer on the same layer


// NOTE: layer merging function returns a buffer that is stored in window.main_buffer and then shown

void H_draw_main_buffer(H_Window window) {

  window.main_buffer = components.buffer[0];


  // return components.buffer[0];
}
















