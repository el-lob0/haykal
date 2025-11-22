#ifndef HAYKAL_H
#define HAYKAL_H

// --- Dependencies ---
// Assumed necessary for types like Pixel and window manipulation
#include <nib.h> 
#include <GLFW/glfw3.h>


// --- Type Definitions ---

// Forward declaration for GLFWwindow, though it's usually defined by <GLFW/glfw3.h>
// typedef struct GLFWwindow GLFWwindow; 

// Represents a UI element index (handle)
typedef int H_Element;

// Defines the layout mode for container elements
typedef enum {
    HORIZONTAL,
    VERTICAL,
} Orientation ;

// Describes the main application window and its root container
typedef struct {
    GLFWwindow *window;
    H_Element child; // index pointing to the box inside which the layout is laid out.
} H_Window;

// Describes the size of an element (used internally)
typedef struct {
    int w;
    int h;
} H_Size;


// --- Window Management Functions ---

/**
 * Creates and initializes a new OS window and its associated H_Window structure.
 * @param title The title of the window.
 * @return The initialized H_Window structure.
 */
H_Window H_new_window(const char *title);



void H_wait();


/**
 * Sets the root container element for the given window.
 * @param pwindow Pointer to the H_Window to modify.
 * @param container The index (H_Element) of the root container element.
 */
void H_set_window_child(H_Window *pwindow, H_Element container);

/**
 * Returns a bool indicating if the OS window is still open.
 * Renamed from H_run_main_loop for clarity in a UI context.
 * @param window The H_Window structure.
 * @return Non-zero if the window is open, 0 otherwise.
 */
int H_main_loop_running(H_Window window);

/**
 * Displays the main buffer of the window after performing the layout calculation.
 * @param pwindow Pointer to the H_Window.
 * @return 0 on success.
 */
int H_show_frame(H_Window *pwindow);

/**
 * Polls OS events (like input, window resize) without waiting.
 */
void H_continue_loop();

/**
 * Pauses the thread until an OS event occurs.
 */
void H_pause_loop_until_event();

/**
 * Posts an empty event to signal the event loop, useful for waking up H_pause_loop_until_event.
 */
void H_send_fake_event();


// --- Element/Component Management Functions ---

/**
 * Creates a new box component (container or widget) and initializes its properties.
 * @param width The width of the element.
 * @param height The height of the element.
 * @param color The background color of the element (if not overwritten by children).
 * @param orientation The layout mode for children (HORIZONTAL or VERTICAL).
 * @param radius The corner radius.
 * @param feather The feather/blur amount.
 * @param padding An array of 4 integers for padding [left, top, right, bottom].
 * @return The index (H_Element) of the newly created box.
 */
H_Element H_new_box(int width, int height, Pixel color, Orientation orientation, int radius, int feather, int padding[4]);

/**
 * Adds a single child element to a parent container.
 * NOTE: This implementation relies on dynamic memory management of the children array 
 * (which isn't shown here) and currently has a potential indexing error (i+1).
 * @param iparent The index (H_Element) of the parent container.
 * @param ichild The index (H_Element) of the child element.
 */
void H_add_child(H_Element iparent, H_Element ichild);

/**
 * Sets the entire array of children for a parent container.
 * WARNING: This function appears to contain a severe logical error in its parent assignment loop.
 * @param iparent The index (H_Element) of the parent container.
 * @param ichildren A pointer to the array of child element indices.
 */
void H_set_children(H_Element iparent, H_Element *ichildren);


#endif // HAYKAL_H
