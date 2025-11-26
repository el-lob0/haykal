#include <GLFW/glfw3.h>
#include "haykal.h"
#include "effects.c"
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
  Anchor *anchor_pos;
  Pixel *color;
} H_Metadata ;

typedef enum {
  HORIZONTAL,
  VERTICAL,
} Vector;

typedef int H_Axis;

typedef struct {
  Vector *vectors;
  int *master;
  int *offset_x; int *offset_y;
  H_Axis **aligned_elements;
  int *seperator;
} Axis;

static Axis axis_anchors = { 0 };

static Margin margins = {0};

void H_update_layers(int new_layer, H_Element index) {
    int current = arrlen(layers.layers);

    if (new_layer >= current) {
        int missing = new_layer - current + 1;

        for (int i = 0; i < missing; i++) {
            int *layer = NULL;   // start empty
            arrpush(layers.layers, layer);
        }
    }

    arrpush(layers.layers[new_layer], index);
}

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
    arrsetcap(components.visibilty, initial_capacity);
    arrsetcap(components.anchor_pos, initial_capacity);

    arrsetcap(layers.layers, initial_capacity);

    arrsetcap(axis_anchors.master, initial_capacity);
    arrsetcap(axis_anchors.offset_x, initial_capacity);
    arrsetcap(axis_anchors.offset_y, initial_capacity);
    arrsetcap(axis_anchors.vectors, initial_capacity);
    arrsetcap(axis_anchors.aligned_elements, initial_capacity);
    arrsetcap(axis_anchors.seperator, initial_capacity);
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
  // components.buffer[0] = nib_rectangle(components.color[0], w, h);
}

// axis also counts as an element so i can tie it to other elements 
H_Axis create_axis(Vector vec, int master, int offset_x, int offset_y, int sep) {

  arrpush(axis_anchors.vectors, vec);

  arrpush(axis_anchors.seperator, sep);
  arrpush(axis_anchors.offset_y, offset_y);
  arrpush(axis_anchors.offset_x, offset_x);

  int *aligned = NULL;
  arrpush(axis_anchors.aligned_elements, aligned);

  arrpush(axis_anchors.master, master);

  return arrlen(axis_anchors.vectors) - 1;
}

/// any element tied to another needs to do so through an axis
void add_to_axis(H_Axis iVec, H_Element iElement) {
  arrpush(axis_anchors.aligned_elements[iVec], iElement);
}

Pixel H_sample_nn(Pixel *buf, int w, int h, float x, float y)
{
    int ix = (int)x;
    int iy = (int)y;

    if (ix < 0 || ix >= w || iy < 0 || iy >= h)
        return (Pixel){0,0,0,0};  

    return buf[iy * w + ix];
}

void H_apply_radius(Pixel *src, int w, int h, int radius) {
  if (radius <= 0) { printf("Invalid OR 0 radius"); return; }

  float r = radius/100.0f; // percentage becomes a 0 to 1 float

  float radius_x = (w/2.f) * r;
  float radius_y = (h/2.f) * r;

  float rx_square = radius_x*radius_x; float ry_square = radius_y*radius_y;

  float cx0 = radius_x, cy0 = radius_y;
  float cx1 = w-radius_x-1, cy0b = radius_y;

  float cx2 = radius_x, cy2=h-radius_y-1;
  float cx3 = w-radius_x-1, cy3 = h-radius_y-1;

  int y; int x;
  for (y=0; y<h; y++) {
    for (x=0; x<w; x++) {
      Pixel *px = &src[y*w+x];

      if (x<radius_x && y < radius_y) {
        float dx = x-cx0;
        float dy = y-cy0;

        if ((dx*dx)/rx_square + (dy*dy)/ry_square>1.0f) {*px = (Pixel){0.0f,0.0f,0.0f,0.0f};}
      }
      if (x>=w-radius_x && y < radius_y) {
        float dx = x-cx1;
        float dy = y-cy0b;

        if ((dx*dx)/rx_square + (dy*dy)/ry_square>1.0f) {*px = (Pixel){0.0f,0.0f,0.0f,0.0f};}
      }
      if (x<radius_x && y >= h-radius_y) {
        float dx = x-cx2;
        float dy = y-cy2;

        if ((dx*dx)/rx_square + (dy*dy)/ry_square>1.0f) {*px = (Pixel){0.0f,0.0f,0.0f,0.0f};}
      }
      if (x>=w-radius_x && y >= h-radius_y) {
        float dx = x-cx3;
        float dy = y-cy3;

        if ((dx*dx)/rx_square + (dy*dy)/ry_square>1.0f) {*px = (Pixel){0.0f,0.0f,0.0f,0.0f};}
      }
    }
  }
}

// element id only needed to update its metadata
Pixel *H_rotate_buffer(Pixel *src, int w, int h, float theta, H_Element id)
{
    float ct = cosf(theta);
    float st = sinf(theta);

    int new_w = fabsf(w * ct) + fabsf(h * st);
    int new_h = fabsf(w * st) + fabsf(h * ct);

    // segfault if not updated
    components.widths[id] = new_w;
    components.heights[id] = new_h;
    

    Pixel *out = nib_init_buffer(new_w, new_h); 

    int cx = new_w / 2;
    int cy = new_h / 2;

    int ox = w / 2;
    int oy = h / 2;

    for (int y = 0; y < new_h; y++) {
        for (int x = 0; x < new_w; x++) {

            float rx = x - cx;
            float ry = y - cy;

            float sx =  rx * ct + ry * st;
            float sy = -rx * st + ry * ct;

            sx += ox;
            sy += oy;

            out[y * new_w + x] = H_sample_nn(src, w, h, sx, sy);
        }
    }

    return out;
}




void my_aa(Pixel *src, int w, int h) {


// NOTE: aa not working but ill move on this is decoration 
}





/// New box is assigned to a layer, given a position and a size.
/// Visibility defaults to 1
H_Element H_new_box(int layer, int width, int height, Pixel color, int angle, int radius, int feather, int x, int y, Anchor anchor ) {

  H_Element ibox = arrlen(components.layer);

  push_metadata(layer, width, height, color, angle, radius, feather, x, y, 1);


  if (components.buffer[ibox] != NULL) { free(components.buffer[ibox]); }
 
  components.buffer[ibox] = nib_rectangle(color, width, height); 

  

  Pixel *buf = components.buffer[ibox];

  components.radius[ibox] = radius;

  H_apply_radius(buf, width, height, radius);

  // my_aa(buf, width, height);

  float rad = angle * (M_PI / 180.f);

  buf = H_rotate_buffer(buf, width, height, rad, ibox);


  // the component buffer becomes a pointer to this buf
  components.buffer[ibox] = buf;

  arrpush(margins.bottom, 0);
  arrpush(margins.top, 0);
  arrpush(margins.right, 0);
  arrpush(margins.left, 0);
  
  H_update_layers(layer, ibox);

  return ibox;
}

void H_set_position(H_Element iElement, int x, int y) {
  components.position_x[iElement] = x;
  components.position_y[iElement] = y;
}

void H_get_position(H_Element iElement, int *x, int *y) {
  *x = components.position_x[iElement];
  *y = components.position_y[iElement];
}

void H_set_dimensions(H_Element iElement, int w, int h) {
  components.widths[iElement] = w;
  components.heights[iElement] = h;
  // FIXME: this needs to modify the actual buffer as well
}

void H_get_dimentions(H_Element iElement, int *w, int *h) {
  *w = components.widths[iElement];
  *h = components.heights[iElement];
}

void H_set_angle(H_Element iElement, int angle) {
  components.angle[iElement] = angle;
  // FIXME: this needs to modify the actual buffer as well
}

void H_get_angle(H_Element iElement, int *angle) {
  *angle = components.angle[iElement];
}

void H_set_visibility(H_Element iElement, int viz) {
  components.visibilty[iElement] = viz;
}

void H_get_visibility(H_Element iElement, int *viz) {
  *viz = components.visibilty[iElement];
}

void H_set_marin(H_Element iElement, int top, int bottom, int right, int left) {
  if (iElement>arrlen(margins.bottom)) { printf("OUT OF BOUNDS MARGIN"); return; }

  margins.top[iElement] = bottom;
  margins.bottom[iElement] = bottom;
  margins.right[iElement] = bottom;
  margins.left[iElement] = bottom;
}


// TODO: check for overlapping buffer on the same layer



typedef struct {
  Pixel *buffer;
  int w;
  int h;
} Core;

static Core main = {
    .buffer = NULL,
    .w = 2000,
    .h = 2000,
};


/* 
 * This function takes nothing
 *
 * it iterates over the array of vectors, checks their master's position,
 * remembers that, and then accordingly sets the followers' absolute positions.
 *
 * */

int modify_positions_to_axis() {
  for (int i = 0; i<arrlen(axis_anchors.master); i++) {
    int iMaster = axis_anchors.master[i];
    int mPosx = components.position_x[iMaster];
    int mPoxy = components.position_y[iMaster];

    // based on orientation, calculate follower positions using seperator that is either through h or w;

  }

  return 0;
}





















int *H_draw_main_buffer(H_Window pWindow) {

  // main.buffer = nib_rectangle((Pixel){0.0f, 0.9f, 0.9f, 0.0f}, main.w, main.h);

  // buffer maker functions allocate space for said buffer
  // if (main.buffer != NULL) { free(main.buffer); }
  if (components.buffer[0] != NULL) { free(components.buffer[0]); }


  components.buffer[0] = nib_rectangle(components.color[0], main.w, main.h);

  int i = layers.layers[1][0];

  int level;
  for (level=0; level<arrlen(layers.layers); level++) {
    int element; int len = arrlen(layers.layers[level]);
    // ELEMENT here is an index to the column that the ELEMENT belongs to in COMPONENTS
    for (element=0; element<len; element++) {
      int i = layers.layers[level][element];
      nib_merge_buffers(components.buffer[0], components.widths[0], components.heights[0], // lowermost layer
                        components.buffer[i], components.widths[i], components.heights[i], // next element in the next layer
                        components.position_x[i], components.position_y[i]); // position to place the element at
    }                   // the position's origin is BOTTOM LEFT
  }
                        // thats how it is with opengl textures
  main.buffer = components.buffer[0];

  return 0;
}














