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
  Pixel *color;
} H_Components ;
// NOTE: I need to malloc at every new addition to the arrays ehehehehe... (man, i miss rust..)

// internal runtime variable so i init it here
static H_Components components = { 0 };
void haykal_init_components(int initial_capacity) {
    
    arrsetcap(components.buffer, initial_capacity);
    arrsetcap(components.children, initial_capacity);
    arrsetcap(components.padding, initial_capacity);
    arrsetcap(components.widths, initial_capacity);
    arrsetcap(components.heights, initial_capacity);
    arrsetcap(components.mode, initial_capacity);
    arrsetcap(components.taken_width, initial_capacity);
    arrsetcap(components.taken_height, initial_capacity);
    arrsetcap(components.parents, initial_capacity);
    arrsetcap(components.child_count, initial_capacity);
    arrsetcap(components.radius, initial_capacity);
    arrsetcap(components.feather, initial_capacity);
    arrsetcap(components.color, initial_capacity);
    
}


// tree gets folded everytime frame changes. so new window size will be available
void layout(H_Element inode, int window_w, int window_h) { 
}










H_Element H_new_box(int width, int height, Pixel color, Orientation orientation, int radius, int feather, int padding[4] ) {

  H_Element ibox;

  ibox = arrlen(components.heights);

  arrpush(components.heights, height);

  printf("%d", ibox); fflush(stdout);
  arrpush(components.buffer, nib_rectangle(color, width, height));
  arrpush(components.color, color);
  arrpush(components.mode, orientation);
  arrpush(components.radius, radius);
  arrpush(components.feather, feather);
  arrpush(components.widths, width);

  // arrsetcap(components.buffer[ibox], width*height);

  arrpush(components.children, NULL);

  int *new_padding = malloc(4 * sizeof(int));
  if (new_padding) {
    memcpy(new_padding, padding, 4 * sizeof(int));
  }
  arrpush(components.padding, new_padding);

  if (ibox == 0) {
    arrpush(components.parents, -1);
  } else {
    arrpush(components.parents, -2);
  }
  // initialized because subject to increments
  arrpush(components.taken_width, 0); arrpush(components.taken_height, 0);

  return ibox;
}

void H_add_child(H_Element iparent, H_Element ichild) {

  arrpush(components.children[iparent], ichild);
  components.parents[ichild] = iparent;
}

void H_set_window_child(H_Window *pwindow, H_Element ichild) {
  pwindow->child = ichild;
  components.parents[ichild] = -1;
}


