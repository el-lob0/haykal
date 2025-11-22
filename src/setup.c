#include <GLFW/glfw3.h>
#include "haykal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stb_ds.h"






/* 

 iname = index of name
 pname = pointer to name

 */

//
// typedef struct {
//   GLFWwindow *window;
//   int container;  // an index in H_Elements, poiting to the box 
//   // inside which the window's layout will be layed out.
// } H_Window;
//
// typedef enum {
//   HORIZONTAL,
//   VERTICAL,
// } Orientation ;
//
// typedef int H_Element; // index of element 



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


void debug(H_Components tree) {
  int i;
  for (i=0;i<components.element_count;i++) {
    printf("%d ", components.feather[i]); fflush(stdout);
    printf("%d ", components.radius[i]); fflush(stdout);
    printf("%d ", components.heights[i]); fflush(stdout);
    printf("%d ", components.widths[i]); fflush(stdout);
  }

}

// tree gets folded everytime frame changes. so new window size will be available
void layout(H_Element inode, int window_w, int window_h) { 

  // case where its a leaf
  if (components.child_count[inode] == 0) {
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
      printf("debug"); fflush(stdout);
      for (row=taken; row<parent_h; row++) {
        for (col=0; col<parent_w; col++) {
          printf("debug"); fflush(stdout);

          int ipx_parent = (row * parent_w) + col;
          int ipx_node = (row-taken)*parent_w + col;
          if (ipx_node > width*height) { break; }
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



void H_set_window_child(H_Window *pwindow, H_Element ichild) {
  pwindow->child = ichild;
  components.parents[ichild] = -1;
}




// NOTE: OHHHH I NEED TO ALLOCATE
// 
// okay good night ill fix this tommorrow
//
// (famous last words)


void haykal_free_elements() {
    for (int i = 0; i <= components.element_count; i++) {
        // Free the padding array for this specific element
        free(components.padding[i]); 
        
        // Free the buffer data for this specific element (if used)
        free(components.buffer[i]);
        
        // Note: You would also need logic to free the child arrays (components.children[i])
    }
}

H_Element H_new_box(int width, int height, Pixel color, Orientation orientation, int radius, int feather, int padding[4] ) {


  int ibox = components.element_count-1;

  components.buffer[ibox] = nib_rectangle(color, width, height);
  components.color[ibox] = color;
  components.mode[ibox] = orientation;
  components.radius[ibox] = radius;
  components.feather[ibox] = feather;
  components.widths[ibox] = width;
  components.heights[ibox] = height;
  if (ibox == 0) {
    components.parents[ibox] = -1;
  }

  
  // initialized because subject to increments
  components.taken_width[ibox] = 0; components.taken_height[ibox] = 0;
  components.child_count[ibox] = 0;

  H_Element out = ibox;
  
  return out;
}

void H_add_child(H_Element iparent, H_Element ichild) {

  int i = components.child_count[iparent];
  components.children[iparent][i+1] = ichild;
  components.parents[ichild] = iparent;
}

void H_set_children(H_Element iparent, H_Element *ichildren) {

  int i = components.child_count[iparent];
  components.children[iparent] = ichildren;
  int s; int c = components.child_count[iparent];
  for (s=0; s<c; s++) {
    components.parents[s] = ichildren[s];
  }
}










