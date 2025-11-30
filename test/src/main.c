// #include "../include/haykal.h"
// #include "../../../include/nib.h"
#define GLAD_GL_IMPLEMENTATION
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <haykal.h>
#include <stdio.h>


int main() {

  H_Window window = H_new_window("LeBaron James");

  H_Color background = H_new_color("#1d376b");
  H_Color foreground = H_new_color("#5db9c2");
  H_Color test = H_new_color("#ffb9c2");
  H_Color text = H_new_color("#260f19");

  H_set_window_bg_color(background);

  H_Element box1 = H_new_box(1, 700, 100, foreground, 0 , 10 , 1, 0, 0, TOP);


  H_Element box2 = H_new_box(2, 100, 100, foreground, 0, 0 , 1, 0, 0, ABSOLUTE);



  H_Font newfont64 = H_add_font("/usr/share/fonts/newfont.ttf", 64);
  H_Font newfont24 = H_add_font("/usr/share/fonts/newfont.ttf", 24);



  H_Element label = H_new_label(3, "test HH\nabcdefgh\nmnopqrstuvw\nxyzABCDEF\nGHIJKLMNO\nPQRSTUVWXYZ",
                                100, 100, 800, 700, text, 24, newfont24);


  H_Element button = H_new_button(3, "HAYKAL IS working lezgooo", 100, 100, 600, 250, 10, text, foreground, 64, newfont64);


  H_Axis axis_the_first = H_create_axis(HORIZONTAL, box1, 0, 0, 50);

  H_add_to_axis(axis_the_first, label);


  H_add_margin(box1, 5, 0, 0, 0); 
  H_add_to_axis(axis_the_first, box2);



  H_monitor_resize(window.window, H_update_size);
  

  while (H_main_loop_running(window)) {

    nib_poll_events();
    H_show_frame(&window);

    if (H_cursor_is_hover(button, window) && H_get_mouse_action() == H_PRESS) {
      printf("button clicked"); fflush(stdout);
    }
  }

  return 0;
}

























