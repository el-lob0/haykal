#include <GLFW/glfw3.h>
#include "haykal.h"
#include "effects.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"
#include "Htext.h"
#include <stdbool.h>


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
  bool *visibilty;
  Anchor *anchor_pos;
  Pixel *color;
} H_Metadata ;

typedef struct {
  char **paths;
} FontPaths;

typedef struct {
  Vector *vectors;
  int *master;
  int *offset_x; int *offset_y;
  H_Axis **aligned_elements;
  int *seperator;
} Axis;

typedef struct {
  H_Element *element_id;
  const char **element_label;
} Labels;

static Labels label_array = { 0 };

static Axis axis_anchors = { 0 };

static Margin margins = {0};

static FontPaths fpaths = {0};

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

    arrsetcap(fpaths.paths, initial_capacity);

    arrsetcap(label_array.element_id, initial_capacity);
    arrsetcap(label_array.element_label, initial_capacity);
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
  arrpush(components.anchor_pos, ABSOLUTE);
  H_update_layers(0, 0);
}

void push_metadata(int layer, int width, Anchor anchor, int height, Pixel color, int angle, int radius, int feather, int x, int y, int is_visible ) {
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
  arrpush(components.anchor_pos, anchor);
}

/// The window's layer number 0, its size depends on the window's size
/// Defaults to gray
void H_update_window_background(H_Color color) {
  components.color[0] = (Pixel)color;
}

void H_update_bg_size(int w, int h) {
  components.widths[0] = w;
  components.heights[0] = h;
  // components.buffer[0] = nib_rectangle(components.color[0], w, h);
}

// axis also counts as an element so i can tie it to other elements 
H_Axis H_create_axis(Vector vec, int master, int offset_x, int offset_y, int sep) {

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
void H_add_to_axis(H_Axis iVec, H_Element iElement) {
  arrpush(axis_anchors.aligned_elements[iVec], iElement);
}

Pixel H_sample_nn(Pixel *buf, int w, int h, float x, float y) {
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
Pixel *H_rotate_buffer(Pixel *src, int w, int h, float theta, H_Element id) {
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

// not void
void H_add_border(Pixel *src, int w, int h, int border) {
  // TODO: add border to metadata and push_metadata
}


void my_aa(Pixel *src, int w, int h) {
}






/// New box is assigned to a layer, given a position and a size.
/// Visibility defaults to 1
H_Element H_new_box(int layer, int width, int height, Pixel color, int angle, int radius, int feather, int x, int y, Anchor anchor ) {

  H_Element ibox = arrlen(components.layer);

  push_metadata(layer, width, anchor, height, color, angle, radius, feather, x, y, 1);


  if (components.buffer[ibox] != NULL) { free(components.buffer[ibox]); }
 
  components.buffer[ibox] = nib_rectangle(color, width, height); 

  

  Pixel *buf = components.buffer[ibox];

  components.radius[ibox] = radius;

  nib_apply_radius(buf, width, height, radius);

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

// TODO: padding

static Font *fonts;
static Atlas *atlas;


H_Font H_add_font(const char *path, int px) {
  H_Font i = arrlen(fonts);
  
  Font new_font = {0};
  Atlas new_atlas = {0};
  if (Hfont_init(&new_font, path, px)) {
    new_atlas = Hbuild_atlas(&new_font);
  }
  arrpush(fonts, new_font);
  arrpush(atlas, new_atlas);

  return i;
}

// currently based on the fontsize 
char *insert_linewrap(const char *unparsed, int char_width, int max_width) {
  int current_width = 0;
  char *target=NULL;

  for (int i=0; unparsed[i] != '\0'; i++) {

    if (current_width+char_width>max_width || current_width+2>max_width) {
      arrpush(target, '\n');
      current_width = 0;
    } 
    arrpush(target, unparsed[i]);
    current_width += char_width;
  }
  arrpush(target, '\0');
  return target;
}

// transparent bg
H_Element H_new_label(int layer, const char *text, int x, int y, int width, int height, Pixel color, int size, H_Font iFont) {
  H_Element ibox = arrlen(components.layer);

  push_metadata(layer, width, ABSOLUTE, height, color, 0, 0, 0, x, y, 1);

  Pixel *buffer_view = nib_init_buffer(width, height);

  // draws text to buffer
  Hrender_text(&atlas[iFont], insert_linewrap(text, size, width), buffer_view, width, height, color, size);

  arrpush(label_array.element_label, text); arrpush(label_array.element_id, ibox);

  components.buffer[ibox] = buffer_view;

  H_update_layers(layer, ibox);

  return ibox;
}


H_Element H_new_button(int layer, const char *label, int x, int y, int width, int height, int radius, Pixel text_color, Pixel bg_color, int size, H_Font iFont) {
  H_Element ibox = arrlen(components.layer);

  push_metadata(layer, width, ABSOLUTE, height, bg_color, 0, radius, 0, x, y, 1);

  Pixel *buffer_view = nib_rectangle( bg_color, width, height);

  // TODO: make offset param 
  Hrender_text(&atlas[iFont], insert_linewrap(label, size, width), buffer_view, width, height, text_color, size);

  nib_apply_radius(buffer_view, width, height, radius);

  arrpush(label_array.element_label, label); arrpush(label_array.element_id, ibox);

  components.buffer[ibox] = buffer_view;

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

void H_set_label(H_Element iElement, const char *label) {

}

void H_set_margin(H_Element iElement, int top, int bottom, int right, int left) {
  if (iElement>arrlen(margins.bottom)) { printf("OUT OF BOUNDS MARGIN"); return; }

  margins.top[iElement] = top;
  margins.bottom[iElement] = bottom;
  margins.right[iElement] = right;
  margins.left[iElement] = left;
}

// for no this just edits the metadata
void H_set_alpha(H_Element iElement, int alpha) {
  float new = (float)alpha/100;
  components.color[iElement].a = new;
  if (components.buffer[iElement] != NULL) { free(components.buffer[iElement]); }
  components.buffer[iElement] = nib_rectangle(components.color[iElement], components.widths[iElement], components.heights[iElement]);
}

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

void H_set_window_size(int width, int height) {
  main.w = width;
  main.h = height;
}

void H_get_window_size(int *width, int *height) {
  *width = main.w;
  *height = main.h;
}

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

    if (axis_anchors.vectors[i] ==  VERTICAL) {
      int last_reserved_y = 0;
      for (int f=0; f<arrlen(axis_anchors.aligned_elements[i]); f++) {

        H_Element iElement = axis_anchors.aligned_elements[i][f];

        components.position_x[iElement] = components.position_x[iMaster]+axis_anchors.offset_x[i];
        components.position_y[iElement] = components.position_y[iMaster]+axis_anchors.offset_y[i] + last_reserved_y;

        if (components.anchor_pos[iMaster] == TOP) {
          components.position_y[iElement] += components.heights[iMaster]-components.heights[iElement];
        }
        if (components.anchor_pos[iMaster] == RIGHT) {
          components.position_x[iElement] += components.widths[iMaster]-components.widths[iElement];
        }

        last_reserved_y += components.heights[iElement] + axis_anchors.seperator[i];
      }
    } else {
      int last_reserved_x = 0;
      for (int f=0; f<arrlen(axis_anchors.aligned_elements[i]); f++) {

        H_Element iElement = axis_anchors.aligned_elements[i][f];

        components.position_x[iElement] = mPosx + axis_anchors.offset_x[i] + last_reserved_x;

        components.position_y[iElement] = mPoxy + axis_anchors.offset_y[i];
        
        if (components.anchor_pos[iMaster] == TOP) {
          components.position_y[iElement] += components.heights[iMaster]-components.heights[iElement];
        }
        if (components.anchor_pos[iMaster] == RIGHT) {
          components.position_x[iElement] += components.widths[iMaster]-components.widths[iElement];
        }

        last_reserved_x += components.widths[iElement] + axis_anchors.seperator[i];
      }
    }
  }
  return 0;
}

int anchor_master() {
  for (int i = 0; i<arrlen(axis_anchors.master); i++) {
    int iMaster = axis_anchors.master[i];

    Anchor anchor = components.anchor_pos[iMaster];

    switch (anchor) {
      
      case TOP: {
       // push the element below the window's top border  untill element is fully visible
        int size_offset = components.heights[iMaster];
        int window_height = main.h;
        int window_width = main.w;

        components.position_x[iMaster] = (window_width-components.widths[iMaster])/2; // to center it 

        components.position_y[iMaster] = window_height-size_offset;
        break;
      };

      case BOTTOM: {
       // push the element *above* the window's *bottom* border  untill element is fully visible
        int size_offset = components.heights[iMaster];
        int window_height = main.h;
        int window_width = main.w;

        components.position_x[iMaster] = (window_width-components.widths[iMaster])/2; // to center it 

        components.position_y[iMaster] = 0; // opengl texture origin is already bottom left, only adjust x position.
        break;
      };

      case RIGHT: {
       // you know the drill by now
        int size_offset = components.widths[iMaster];
        int window_height = main.h;
        int window_width = main.w;

        components.position_x[iMaster] = window_width-size_offset;  

        components.position_y[iMaster] = (window_height-components.heights[iMaster])/2;
        break;
      };

      case LEFT: {
        int size_offset = components.widths[iMaster];
        int window_height = main.h;
        int window_width = main.w;

        components.position_x[iMaster] = 0;  // opengl texture origin is already bottom left, only adjust x position. 

        components.position_y[iMaster] = (window_height-components.heights[iMaster])/2;
        break;
      };

      default: ; // position is absolute, dont mess with it (kuroko no basket reference)
    }
  }

  return 0;
}

int update_labels() {
  for (int i=0; i<arrlen(label_array.element_id); i++) {
    int iElement = label_array.element_id[i];
    const char *label = label_array.element_label[i];

    if (components.buffer[i] != NULL) { free(components.buffer[i]); }
    components.buffer[i] = nib_rectangle(components.color[i], components.widths[i], components.heights[i]);

    // TODO: update buffer with new text
    // Hrender_text(&atlas[], label, components.buffer[i], components.widths[i], 
                 // components.heights[i], components.color[i], 64);
  }
  return 0;
}

int apply_margins() { 
    for (int i = 0; i < arrlen(margins.top); i++) {
        components.position_x[i] += margins.left[i];
        components.position_y[i] -= margins.top[i];
        components.position_x[i] -= margins.right[i];
        components.position_y[i] += margins.bottom[i];
    }
  return 0;
}

void H_add_margin(H_Element iElement, int top, int bottom, int left, int right) {
  margins.left[iElement] = left;
  margins.top[iElement] = top;
  margins.right[iElement] = right;
  margins.bottom[iElement] = bottom;

}



// EVENTS


typedef enum {
  PRESSED,
  HELD,
  RELEASED
} KeyState;

typedef struct {
  double cursor_x;
  double cursor_y;
  int mouse_button;
  int mouse_action;
  double scroll_offset_x;
  double scroll_offset_y;
  int key;
  KeyState key_state;

} Events;

static Events event_box = {0};

// MOUSE CLICK
void mouse_callback(GLFWwindow *window, int button, int action, int mods) {
  event_box.mouse_button = button;
  event_box.mouse_action = action;
  printf("callback"); fflush(stdout);
}

int H_get_mouse_action() {
  return event_box.mouse_action;
}

int H_get_mouse_button() {
  return event_box.mouse_button;
}

// CURSOR POSITION
void cursor_callback( GLFWwindow *window, double x, double y) {
  event_box.cursor_x = main.w - x;
  event_box.cursor_y = main.h - y;
}

void H_get_cursor_pos(int *x, int *y) {
  *x = (int)event_box.cursor_x;
  *y = (int)event_box.cursor_y;
}

bool H_cursor_is_hover(H_Element iElement, H_Window window) {
  // range of the element to be hovered
  int x_start, x_end;
  int y_start, y_end;

  x_start = components.position_x[iElement]; y_start = components.position_y[iElement];
  x_end = components.position_x[iElement] + components.widths[iElement];
  y_end = components.position_y[iElement] + components.heights[iElement];
  // printf("start x: %d start y: %d \n end x: %d end y: %d \n \n", x_start, y_start, x_end, y_end); fflush(stdout);
  // printf("cursor x: %d cursor y: %d \n \n \n", (int)event_box.cursor_x, (int)event_box.cursor_y); fflush(stdout);

  if (x_start <= event_box.cursor_x 
      && event_box.cursor_x <= x_end 
      && y_start <= event_box.cursor_y
      && event_box.cursor_y <= y_end) 
  {
    return true;
  }
  else { return false; }
}

// SCROLL

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  event_box.scroll_offset_x = xoffset;
  event_box.scroll_offset_y = yoffset;
} 

void H_get_scroll_event(H_Window window, int *xoffset, int *yoffset) {
  *xoffset = event_box.scroll_offset_x;
  *yoffset = event_box.scroll_offset_y;
}

void H_clear_events() {
  event_box.mouse_action = 0;
  event_box.scroll_offset_x = 0;
  event_box.scroll_offset_y = 0;
}

// KEYBOARD

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  switch (action) {
    case GLFW_PRESS: {event_box.key = key; event_box.key_state = PRESSED; break; };
    case GLFW_RELEASE: {event_box.key = key; event_box.key_state = RELEASED; break; };
    case GLFW_REPEAT: {event_box.key = key; event_box.key_state = HELD; break; };
  }
}

bool H_key_press_event(GLFWwindow* window) {
  if (event_box.key_state == PRESSED) { return true; }
  return false;
}

bool H_key_release_event(GLFWwindow* window) {
  if (event_box.key_state == RELEASED) { return true; }
  return false;
}

bool H_key_hold_event(GLFWwindow* window) {
  if (event_box.key_state == HELD) { return true; }
  return false;
}

int H_get_key() {
  return event_box.key;
}


bool H_element_clicked(H_Element iElement, H_Window window) {
    if (H_cursor_is_hover(iElement, window) && event_box.mouse_action == H_PRESS && event_box.mouse_button == H_MOUSE_LEFT) {
        return true;
    } else {
      return false;
    }
}


// TODO: EVENTS: clipboard input, path drop input, time input.

// COLORS


H_Color H_new_color(const char *hex) {
    H_Color color = {0.5f, 0.0f, 0.0f, 1.0f}; // default is opaque reddish

    if (!hex || hex[0] != '#') return color;

    size_t len = strlen(hex);
    unsigned int r = 0, g = 0, b = 0, a = 255;

    if (len == 7) { // #RRGGBB
        sscanf(hex + 1, "%02x%02x%02x", &r, &g, &b);
    } else if (len == 9) { // #RRGGBBAA
        sscanf(hex + 1, "%02x%02x%02x%02x", &r, &g, &b, &a);
    }

    color.r = r / 255.0f;
    color.g = g / 255.0f;
    color.b = b / 255.0f;
    color.a = a / 255.0f;

    return color;
}

// ------------------------------------------------------------
// MAIN

int *H_draw_main_buffer(H_Window pWindow) {


  if (components.buffer[0] != NULL) { free(components.buffer[0]); }


  components.buffer[0] = nib_rectangle(components.color[0], main.w, main.h);

  anchor_master();
  apply_margins();
  modify_positions_to_axis();

  int level;
  for (level=0; level<arrlen(layers.layers); level++) {
    int element; int len = arrlen(layers.layers[level]);
    // ELEMENT here is an index to the column that the ELEMENT belongs to in COMPONENTS
    for (element=0; element<len; element++) {
      int i = layers.layers[level][element];

      if (components.visibilty[i] == false) { continue; }

      nib_merge_buffers(components.buffer[0], components.widths[0], components.heights[0], // lowermost layer
                        components.buffer[i], components.widths[i], components.heights[i], // next element in the next layer
                        components.position_x[i], components.position_y[i]); // position to place the element at
    }                   // the position's origin is BOTTOM LEFT
  }
                        // thats how it is with opengl textures
  main.buffer = components.buffer[0];

  return 0;
}














