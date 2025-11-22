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
  int element_count;
} H_Components ;
// NOTE: I need to malloc at every new addition to the arrays ehehehehe... (man, i miss rust..)

// internal runtime variable so i init it here
static H_Components components = { .element_count = -1, };
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
    
    components.element_count = -1; 
}


// tree gets folded everytime frame changes. so new window size will be available
void layout(H_Element inode, int window_w, int window_h) { 

  // case where its a leaf
  if (arrlen(components.children[inode]) == 0) {
    int width = components.widths[inode];
    int height = components.heights[inode];
    
    int iparent = components.parents[inode];
    
    int parent_w; int parent_h; int taken;

    Pixel *child_buffer = nib_init_buffer(parent_w, parent_h);

    if (iparent == -1) {
    // when the parent is -1 that means the current node is a direct child of the window
      parent_w = window_w;
      parent_h = window_h;

      int col; int row;

      printf("debug"); fflush(stdout);
      for (row=taken; row<parent_h; row++) {
        for (col=0; col<parent_w; col++) {

          int ipx_parent = (row * parent_w) + col;
          int ipx_node = (row-taken)*parent_w + col;
          if (ipx_node > width*height) { break; }
          arrsetcap(components.buffer[inode], 64);
          child_buffer[ipx_parent] = components.buffer[inode][ipx_node];
        }
      }
    } else {

      taken = 1;
      parent_w = components.widths[iparent];
      parent_h = components.heights[iparent];

      if (components.mode[iparent] == VERTICAL || iparent == -1) {
        
        int col; int row;

        printf("debug"); fflush(stdout);
        for (row=taken; row<parent_h; row++) {
          for (col=0; col<parent_w; col++) {
            printf("debug"); fflush(stdout);

            int ipx_parent = (row * parent_w) + col;
            int ipx_node = (row-taken)*parent_w + col;
            if (ipx_node > width*height) { break; }
            arrsetcap(components.buffer[inode], 64);
            child_buffer[ipx_parent] = components.buffer[inode][ipx_node];
          }
        }
      } else {
        if (taken ==1) { taken = components.taken_width[iparent]; } // x number of cols from the first col 
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
    }
    // and then i do the merging here
 } 
 // case where its a node
  else {
    int *children = components.children[inode];
    int i;
    for (i=0; i<components.child_count[inode]; i++) {
      layout(components.children[inode][i], window_w, window_h);
    }
  }
}

H_Element H_new_box(int width, int height, Pixel color, Orientation orientation, int radius, int feather, int padding[4] ) {

  H_Element ibox;

  ibox = arrpush(components.heights, height);

  arrpush(components.buffer, nib_rectangle(color, width, height));
  arrpush(components.color, color);
  arrpush(components.mode, orientation);
  arrpush(components.radius, radius);
  arrpush(components.feather, feather);
  arrpush(components.widths, width);

  arrsetcap(components.buffer[ibox], width*height);

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


