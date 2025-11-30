// #include "../include/haykal.h"
// #include "../../../include/nib.h"
#define GLAD_GL_IMPLEMENTATION
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <haykal.h>
#include <stdio.h>




int main() {

  H_Window window = H_new_window("Calculator");

  H_Color background = H_new_color("#1d376b");
  H_Color button = H_new_color("#5db9c2");
  H_Color transparent = H_new_color("#00000000");
  H_Color text = H_new_color("#260f19");

  H_set_window_bg_color(background);

  H_Font newfont64 = H_add_font("/usr/share/fonts/newfont.ttf", 64);
  H_Font newfont24 = H_add_font("/usr/share/fonts/newfont.ttf", 24);





  // we will set this to follow the size of the window so position has to match origin
  H_Element main_container = H_new_box(1, 700, 100, background, 0 , 10 , 1, 0, 0, ABSOLUTE);


  H_Element result_display = H_new_label(2, "Hello World!", 100, 100, 800, 700, text, 64, newfont64);


  H_Element button_1 = H_new_button(2, " 1", 0, 0, 100, 100, 4, text, button, 64, newfont64);
  H_Element button_2 = H_new_button(2, " 2", 0, 0, 100, 100, 4, text, button, 64, newfont64);
  H_Element button_3 = H_new_button(2, " 3", 0, 0, 100, 100, 4, text, button, 64, newfont64);
  H_Element button_4 = H_new_button(2, " 4", 0, 0, 100, 100, 4, text, button, 64, newfont64);
  H_Element button_5 = H_new_button(2, " 5", 0, 0, 100, 100, 4, text, button, 64, newfont64);
  H_Element button_6 = H_new_button(2, " 6", 0, 0, 100, 100, 4, text, button, 64, newfont64);
  H_Element button_7 = H_new_button(2, " 7", 0, 0, 100, 100, 4, text, button, 64, newfont64);
  H_Element button_8 = H_new_button(2, " 8", 0, 0, 100, 100, 4, text, button, 64, newfont64);
  H_Element button_9 = H_new_button(2, " 9", 0, 0, 100, 100, 4, text, button, 64, newfont64);
  H_Element button_0 = H_new_button(2, " 0", 0, 0, 100, 100, 4, text, button, 64, newfont64);
  H_Element button_equal = H_new_button(2, " =", 0, 0, 100, 100, 4, text, button, 64, newfont64);
  H_Element button_plus = H_new_button(2, " +", 0, 0, 100, 100, 4, text, button, 64, newfont64);
  // H_Element button_minus = H_new_button(2, " -", 0, 0, 100, 100, 4, text, button, 64, newfont64);
  // H_Element button_times = H_new_button(2, " *", 0, 0, 100, 100, 4, text, button, 64, newfont64);
  // H_Element button_divide = H_new_button(2, " /", 0, 0, 100, 100, 4, text, button, 64, newfont64);



  H_Element row_1 = H_new_box(3, 500, 100, transparent, 0, 0, 0, 0, 0, ABSOLUTE);
  H_Element row_2 = H_new_box(3, 500, 100, transparent, 0, 0, 0, 0, 0, ABSOLUTE);
  H_Element row_3 = H_new_box(3, 500, 100, transparent, 0, 0, 0, 0, 0, ABSOLUTE);
  H_Element row_4 = H_new_box(3, 500, 100, transparent, 0, 0, 0, 0, 0, ABSOLUTE);

  H_Axis grid_y_axis = H_create_axis(VERTICAL, main_container, 0, 0, 5);
  H_Axis grid_x1_axis = H_create_axis(HORIZONTAL, row_1, 0, 0, 5);
  H_Axis grid_x2_axis = H_create_axis(HORIZONTAL, row_2, 0, 0, 5);
  H_Axis grid_x3_axis = H_create_axis(HORIZONTAL, row_3, 0, 0, 5);
  H_Axis grid_x4_axis = H_create_axis(HORIZONTAL, row_4, 0, 0, 5);

  H_add_to_axis(grid_y_axis, result_display);
  H_add_to_axis(grid_y_axis, row_1);
  H_add_to_axis(grid_y_axis, row_2);
  H_add_to_axis(grid_y_axis, row_3);
  H_add_to_axis(grid_y_axis, row_4);

  H_add_to_axis(grid_x1_axis, button_1);
  H_add_to_axis(grid_x1_axis, button_2);
  H_add_to_axis(grid_x1_axis, button_3);

  H_add_to_axis(grid_x2_axis, button_4);
  H_add_to_axis(grid_x2_axis, button_5);
  H_add_to_axis(grid_x2_axis, button_6);

  H_add_to_axis(grid_x3_axis, button_7);
  H_add_to_axis(grid_x3_axis, button_8);
  H_add_to_axis(grid_x3_axis, button_9);

  H_add_to_axis(grid_x4_axis, button_0);
  H_add_to_axis(grid_x4_axis, button_plus);
  H_add_to_axis(grid_x4_axis, button_equal);



  int frame = 0;

  bool flag = false;

  H_monitor_resize(window.window, H_update_size);
  

  while (H_main_loop_running(window)) {

    
    H_poll_events(); 
    H_show_frame(&window);

    H_clear_events();
  }


  return 0;
}

























