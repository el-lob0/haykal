#ifndef HAYKAL_H
#define HAYKAL_H

#include <nib.h> 
#include <GLFW/glfw3.h>
#include <stdbool.h>



#define H_pause_loop_until_event() nib_wait_events()
 

typedef int H_Element;

typedef struct {
    GLFWwindow *window;
    H_Element origin;
    Pixel *main_buffer; 
    int buffer_w;
    int buffer_h;
} H_Window;

typedef struct {
    int w;
    int h;
} H_Size;

void H_add_margin(H_Element iElement, int top, int bottom, int left, int right);

void H_set_margin(H_Element iElement, int top, int bottom, int right, int left);

#define H_monitor_resize(win, func) nib_read_window_size(win, func)

H_Window H_new_window(const char *title);

void H_update_size( GLFWwindow *window, int w, int h );

void H_wait();

void haykal_init_components(H_Window window, int initial_capacity); 

void H_pause_until_event();

int H_main_loop_running(H_Window window);

bool H_element_clicked(H_Element iElement, H_Window window);

#define H_Color Pixel

void H_update_window_background(H_Color color);

#define H_set_window_bg_color(color) H_update_window_background(color)

int H_show_frame(H_Window *pwindow);


void H_continue_loop();


void H_pause_loop_until_event();


void H_send_fake_event();

typedef int H_Axis;

typedef enum {
  HORIZONTAL,
  VERTICAL,
} Vector;

H_Axis H_create_axis(Vector vec, int master, int offset_x, int offset_y, int sep);

void H_add_to_axis(H_Axis iVec, H_Element iElement);

typedef struct {
  int *top;
  int *bottom;
  int *right;
  int *left;
} Margin;

typedef enum {
  TOP,
  RIGHT,
  BOTTOM,
  LEFT,
  ABSOLUTE
} Anchor;

H_Element H_new_box(int layer, int width, int height, Pixel color, int angle, int radius, int feather, int x, int y, Anchor anchor );

typedef int H_Font;

H_Font H_add_font(const char *path, int px);

H_Element H_new_label(int layer, const char *text, int x, int y, int width, int height, Pixel color, int size, H_Font iFont);


// heh 
#define H_KEY_SPACE	GLFW_KEY_SPACE   
 
#define H_KEY_APOSTROPHE	GLFW_KEY_APOSTROPHE   
 
#define H_KEY_COMMA	GLFW_KEY_COMMA   
 
#define H_KEY_MINUS	GLFW_KEY_MINUS   
 
#define H_KEY_PERIOD	GLFW_KEY_PERIOD   
 
#define H_KEY_SLASH	GLFW_KEY_SLASH   
 
#define H_KEY_0	GLFW_KEY_0   
 
#define H_KEY_1	GLFW_KEY_1   
 
#define H_KEY_2	GLFW_KEY_2   
 
#define H_KEY_3	GLFW_KEY_3   
 
#define H_KEY_4	GLFW_KEY_4   
 
#define H_KEY_5	GLFW_KEY_5   
 
#define H_KEY_6	GLFW_KEY_6   
 
#define H_KEY_7	GLFW_KEY_7   
 
#define H_KEY_8	GLFW_KEY_8   
 
#define H_KEY_9	GLFW_KEY_9   
 
#define H_KEY_SEMICOLON	GLFW_KEY_SEMICOLON 
 
#define H_KEY_EQUAL	GLFW_KEY_EQUAL 
 

#define H_KEY_A                GLFW_KEY_A
#define H_KEY_B                GLFW_KEY_B
#define H_KEY_C                GLFW_KEY_C
#define H_KEY_D                GLFW_KEY_D
#define H_KEY_E                GLFW_KEY_E
#define H_KEY_F                GLFW_KEY_F
#define H_KEY_G                GLFW_KEY_G
#define H_KEY_H                GLFW_KEY_H
#define H_KEY_I                GLFW_KEY_I
#define H_KEY_J                GLFW_KEY_J
#define H_KEY_K                GLFW_KEY_K
#define H_KEY_L                GLFW_KEY_L
#define H_KEY_M                GLFW_KEY_M
#define H_KEY_N                GLFW_KEY_N
#define H_KEY_O                GLFW_KEY_O
#define H_KEY_P                GLFW_KEY_P
#define H_KEY_Q                GLFW_KEY_Q
#define H_KEY_R                GLFW_KEY_R
#define H_KEY_S                GLFW_KEY_S
#define H_KEY_T                GLFW_KEY_T
#define H_KEY_U                GLFW_KEY_U
#define H_KEY_V                GLFW_KEY_V
#define H_KEY_W                GLFW_KEY_W
#define H_KEY_X                GLFW_KEY_X
#define H_KEY_Y                GLFW_KEY_Y
#define H_KEY_Z                GLFW_KEY_Z

#define H_KEY_LEFT_BRACKET     GLFW_KEY_LEFT_BRACKET
#define H_KEY_BACKSLASH        GLFW_KEY_BACKSLASH
#define H_KEY_RIGHT_BRACKET    GLFW_KEY_RIGHT_BRACKET
#define H_KEY_GRAVE_ACCENT     GLFW_KEY_GRAVE_ACCENT

#define H_KEY_WORLD_1          GLFW_KEY_WORLD_1
#define H_KEY_WORLD_2          GLFW_KEY_WORLD_2

#define H_KEY_ESCAPE           GLFW_KEY_ESCAPE
#define H_KEY_ENTER            GLFW_KEY_ENTER
#define H_KEY_TAB              GLFW_KEY_TAB
#define H_KEY_BACKSPACE        GLFW_KEY_BACKSPACE

#define H_KEY_INSERT           GLFW_KEY_INSERT
#define H_KEY_DELETE           GLFW_KEY_DELETE
#define H_KEY_RIGHT            GLFW_KEY_RIGHT
#define H_KEY_LEFT             GLFW_KEY_LEFT
#define H_KEY_DOWN             GLFW_KEY_DOWN
#define H_KEY_UP               GLFW_KEY_UP

#define H_KEY_PAGE_UP          GLFW_KEY_PAGE_UP
#define H_KEY_PAGE_DOWN        GLFW_KEY_PAGE_DOWN
#define H_KEY_HOME             GLFW_KEY_HOME
#define H_KEY_END              GLFW_KEY_END

#define H_KEY_CAPS_LOCK        GLFW_KEY_CAPS_LOCK
#define H_KEY_SCROLL_LOCK      GLFW_KEY_SCROLL_LOCK
#define H_KEY_NUM_LOCK         GLFW_KEY_NUM_LOCK
#define H_KEY_PRINT_SCREEN     GLFW_KEY_PRINT_SCREEN
#define H_KEY_PAUSE            GLFW_KEY_PAUSE

#define H_KEY_F1               GLFW_KEY_F1
#define H_KEY_F2               GLFW_KEY_F2
#define H_KEY_F3               GLFW_KEY_F3
#define H_KEY_F4               GLFW_KEY_F4
#define H_KEY_F5               GLFW_KEY_F5
#define H_KEY_F6               GLFW_KEY_F6
#define H_KEY_F7               GLFW_KEY_F7
#define H_KEY_F8               GLFW_KEY_F8
#define H_KEY_F9               GLFW_KEY_F9
#define H_KEY_F10              GLFW_KEY_F10
#define H_KEY_F11              GLFW_KEY_F11
#define H_KEY_F12              GLFW_KEY_F12
#define H_KEY_F13              GLFW_KEY_F13
#define H_KEY_F14              GLFW_KEY_F14
#define H_KEY_F15              GLFW_KEY_F15
#define H_KEY_F16              GLFW_KEY_F16
#define H_KEY_F17              GLFW_KEY_F17
#define H_KEY_F18              GLFW_KEY_F18
#define H_KEY_F19              GLFW_KEY_F19
#define H_KEY_F20              GLFW_KEY_F20
#define H_KEY_F21              GLFW_KEY_F21
#define H_KEY_F22              GLFW_KEY_F22
#define H_KEY_F23              GLFW_KEY_F23
#define H_KEY_F24              GLFW_KEY_F24
#define H_KEY_F25              GLFW_KEY_F25

#define H_KEY_KP_0             GLFW_KEY_KP_0
#define H_KEY_KP_1             GLFW_KEY_KP_1
#define H_KEY_KP_2             GLFW_KEY_KP_2
#define H_KEY_KP_3             GLFW_KEY_KP_3
#define H_KEY_KP_4             GLFW_KEY_KP_4
#define H_KEY_KP_5             GLFW_KEY_KP_5
#define H_KEY_KP_6             GLFW_KEY_KP_6
#define H_KEY_KP_7             GLFW_KEY_KP_7
#define H_KEY_KP_8             GLFW_KEY_KP_8
#define H_KEY_KP_9             GLFW_KEY_KP_9

#define H_KEY_KP_DECIMAL       GLFW_KEY_KP_DECIMAL
#define H_KEY_KP_DIVIDE        GLFW_KEY_KP_DIVIDE
#define H_KEY_KP_MULTIPLY      GLFW_KEY_KP_MULTIPLY
#define H_KEY_KP_SUBTRACT      GLFW_KEY_KP_SUBTRACT
#define H_KEY_KP_ADD           GLFW_KEY_KP_ADD
#define H_KEY_KP_ENTER         GLFW_KEY_KP_ENTER
#define H_KEY_KP_EQUAL         GLFW_KEY_KP_EQUAL

#define H_KEY_LEFT_SHIFT       GLFW_KEY_LEFT_SHIFT
#define H_KEY_LEFT_CONTROL     GLFW_KEY_LEFT_CONTROL
#define H_KEY_LEFT_ALT         GLFW_KEY_LEFT_ALT
#define H_KEY_LEFT_SUPER       GLFW_KEY_LEFT_SUPER

#define H_KEY_RIGHT_SHIFT      GLFW_KEY_RIGHT_SHIFT
#define H_KEY_RIGHT_CONTROL    GLFW_KEY_RIGHT_CONTROL
#define H_KEY_RIGHT_ALT        GLFW_KEY_RIGHT_ALT
#define H_KEY_RIGHT_SUPER      GLFW_KEY_RIGHT_SUPER

#define H_KEY_MENU             GLFW_KEY_MENU
#define H_KEY_LAST             GLFW_KEY_MENU

#define H_PRESS  1
#define H_RELEASE  GLFW_RELEASE

#define H_MOUSE_LEFT GLFW_MOUSE_BUTTON_LEFT
#define H_MOUSE_RIGHT GLFW_MOUSE_BUTTON_RIGHT
#define H_MOUSE_MIDDLE GLFW_MOUSE_BUTTON_MIDDLE


typedef enum {
  LEFT_CLICK,
  RIGHT_CLICK,
  WHEEL_CLICK,
  NO_CLICK,
} MouseEvent;

MouseEvent H_mouse_click_event(H_Window window);

void H_get_cursor_pos(int *x, int *y);

bool H_cursor_is_hover(H_Element iElement, H_Window window);

void H_get_scroll_event(H_Window window, int *xoffset, int *yoffset);

bool H_key_press_event(GLFWwindow* window);
bool H_key_release_event(GLFWwindow* window);
bool H_key_hold_event(GLFWwindow* window);

int H_get_key();

int H_get_mouse_action();

int H_get_mouse_button();


H_Color H_new_color(const char *hex);


H_Element H_new_button(int layer, const char *label, int x, int y, int width, int height, int radius, Pixel text_color, Pixel bg_color, int size, H_Font iFont); 


#endif 
