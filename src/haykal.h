
#ifndef HAYKAL_H
#define HAYKAL_H

#include <nib.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include "Hmacros.h"

/// These types are IDs used to retrieve the metadata of the different elements
/// An element is any visual item with metadata that describes how it should be drawn  
typedef int H_Element;
/// An axis points is what enables the alignement of elements
/// and enabling elements to follow the position of other master elements
typedef int H_Axis;
/// A font is an instance of a font at a certain size.
typedef int H_Font;

// Private struct
typedef struct {
    GLFWwindow *window;
    H_Element origin;
    Pixel *main_buffer;
    int buffer_w;
    int buffer_h;
} H_Window;

// Garbage
typedef struct {
    int w;
    int h;
} H_Size;

/// Struct of Arrays containing margins
typedef struct {
    int *top;
    int *bottom;
    int *right;
    int *left;
} Margin;

/// Which side of the screen tho stick an element to, or an absolute position
typedef enum {
    TOP,
    RIGHT,
    BOTTOM,
    LEFT,
    ABSOLUTE
} Anchor;

/// What orientation to align the axis elements to.
typedef enum {
    HORIZONTAL,
    VERTICAL,
} Vector;

typedef enum {
    LEFT_CLICK,
    RIGHT_CLICK,
    WHEEL_CLICK,
    NO_CLICK,
} MouseEvent;

/// Mandatory initialization, looping, and event functions
H_Window H_new_window(const char *title);
void H_update_size(GLFWwindow *window, int w, int h);
int H_main_loop_running(H_Window window);
int H_show_frame(H_Window *pwindow);
void H_wait();

/// Self explanatory
void H_set_window_size(int width, int height);
void H_get_window_size(int *width, int *height);

/// Disfunctional for now
void H_pause_until_event();
void H_continue_loop();

/// Also disfunctional
#define H_pause_loop_until_event() nib_wait_events()

/// Private function
void haykal_init_components(H_Window window, int initial_capacity);

/// Takes a reference to 2 ints and an element and assigns the x and y positions of the element asked to them
void H_get_position(H_Element iElement, int *x, int *y);

/// Creates a rectangle as an element, feather argument is currently obsolete
H_Element H_new_box(
    int layer, int width, int height, Pixel color,
    int angle, int radius, int feather,
    int x, int y, Anchor anchor
);

/// For dynamically editing the margin of elements
void H_set_margin(H_Element iElement, int top, int bottom, int left, int right);
/// For adding a margin as well.
void H_add_margin(H_Element iElement, int top, int bottom, int left, int right);

/// To change the alpha channel of an element. Useful for fading.
void H_set_alpha(H_Element iElement, int alpha);

/// Creates a font instance of a certain size.
H_Font H_add_font(const char *path, int px);

/// New transparent background label
H_Element H_new_label(
    int layer, const char *text,
    int x, int y,
    int width, int height,
    Pixel color, int size,
    H_Font iFont
);

/// Edits the label of an element
void H_set_label(H_Element iElement, const char *label);

/// Functionally it is just a label with a background color
H_Element H_new_button(
    int layer, const char *label,
    int x, int y, int width, int height,
    int radius,
    Pixel text_color, Pixel bg_color,
    int size, H_Font iFont
);

/// Axis creation, needs to be tied to a master at creation
H_Axis H_create_axis(Vector vec, int master, int offset_x, int offset_y, int sep);
/// Add an element to an axis, order of adding affects order of alignement
void H_add_to_axis(H_Axis iVec, H_Element iElement);

/// Used to keep the loop running
#define H_poll_events() glfwPollEvents()

/// Used to keep event states dynamic, necessary.
void H_clear_events();
/// Can be used as some sort of signal (not yet implemented)
void H_send_fake_event();

/// Disfunctional, use example instead
bool H_element_clicked(H_Element iElement, H_Window window);

/// Obsolete
MouseEvent H_mouse_click_event(H_Window window);

/// Takes a reference to an x and y and assigns the x and y positions of the cursor to them
void H_get_cursor_pos(int *x, int *y);
/// To check if an element is hovered during the main loop
bool H_cursor_is_hover(H_Element iElement, H_Window window);

/// Obsolete as there is no scrollview yet
void H_get_scroll_event(H_Window window, int *xoffset, int *yoffset);

/// key events, not tested
bool H_key_press_event(GLFWwindow* window);
bool H_key_release_event(GLFWwindow* window);
bool H_key_hold_event(GLFWwindow* window);

/// Not tested
int H_get_key();
int H_get_mouse_action();
int H_get_mouse_button();

/// Allows for easier theming
#define H_Color Pixel

/// Usefull getters and setters
void H_set_position(H_Element iElement, int x, int y);
void H_set_dimensions(H_Element iElement, int w, int h);
void H_get_dimentions(H_Element iElement, int *w, int *h);
void H_set_angle(H_Element iElement, int angle);
void H_get_angle(H_Element iElement, int *angle);
void H_set_visibility(H_Element iElement, int viz);
void H_get_visibility(H_Element iElement, int *viz);


/// to set the window background color use at initialization
void H_update_window_background(H_Color color);
#define H_set_window_bg_color(color) H_update_window_background(color)

/// Turns a hex code into a color
H_Color H_new_color(const char *hex);

/// necessary to keep the window background consistent with window size
#define H_monitor_resize(win, func) nib_read_window_size(win, func)

#endif

