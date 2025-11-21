#include <GLFW/glfw3.h>
#include "haykal.h"


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
 } 
 // case where its a node
 else {
    int *children = components.children[inode];
    int i;
    for (i=0; i<components.child_count[inode]; i++) {
      layout(components.children[inode][i]);
    }
  }
}



void H_set_window_child(H_Window *pwindow, int container) {
  pwindow->container = container;
}




// NOTE: OHHHH I NEED TO ALLOCATE
// 
// okay good night ill fix this tommorrow
//
// (famous last words)

H_Element H_new_box(int width, int height, Pixel color, Orientation orientation, int radius, int feather, int padding[4] ) {
  int ibox = components.element_count + 1;

  components.buffer[ibox] = nib_rectangle(color, width, height);
  components.color[ibox] = color;
  components.mode[ibox] = orientation;
  components.radius[ibox] = radius;
  components.feather[ibox] = feather;
  components.padding[ibox] = padding;
  
  // initialized because subject to increments
  components.taken_width[ibox] = 0; components.taken_height[ibox] = 0;
  components.child_count[ibox] = 0;
  components.element_count = ibox;

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










