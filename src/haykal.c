#include <stdio.h>
#include "../include/haykal.h"
#include "../include/glad.c"
#include "../include/nib.h"
#include "util.c"
#include "setup.c"
#define GLAD_GL_IMPLEMENTATION
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdlib.h>





#include "haykal.h" 
#include <stdio.h>
#include <time.h>


double g_animation_progress = 0.0;
double g_animation_start_time = 0.0;
double g_animation_duration = 0.5; 
bool g_is_animating = false;

int f_timing(double x) { 
    return (int)(1.0 - exp(-10.0 * x)); 
}


void start_overlay_animation(hkl_widget_t* overlay) {
    if (!g_is_animating) {
        g_is_animating = true;
        g_animation_start_time = hkl_get_time(); 
        g_animation_progress = 0.0;
        printf("Animation started.\n");
    }
}

void on_key_press(hkl_widget_t* overlay, int key) {
    if (key == 'A') {
        start_overlay_animation(overlay);
    }
}

void update_animations(hkl_widget_t* overlay) {
    if (g_is_animating) {
        double current_time = hkl_get_time();
        double elapsed = current_time - g_animation_start_time;
        
        double t = (elapsed > g_animation_duration) ? 1.0 : (elapsed / g_animation_duration);
        
        int eased_value = f_timing(t); 
        
        int new_size = 50 + (int)(50.0 * eased_value);
        
        hkl_set_widget_size(overlay, new_size, new_size);
        
        if (t >= 1.0) {
            g_is_animating = false;
            printf("Animation finished. Final size: %d\n", new_size);
        }
    }
}



int main(void) {
    hkl_window_t* window = hkl_new_window("HKL Manual Loop Example");
    
    hkl_widget_t* box = hkl_new_box("MainBox", "h", "#2C3E50", "stick-top-left");
    hkl_set_window_children(window, box);

    hkl_widget_t* other_box = hkl_new_box("Content", "h", "#E74C3C", "stick-top-left", "label", "Click 'A'!");
    hkl_shape_t* shadow = hkl_new_shape("rectangle", 100, "black", 50, "top left", 50, 50); // Initial size 50x50
    hkl_widget_t* overlay = hkl_new_overlay_box(shadow, other_box);
    
    hkl_widget_t* label = hkl_new_label("StatusLabel", "Status: Ready");
    hkl_box_set_children(box, overlay, label); 

    hkl_set_key_callback(window, overlay, on_key_press); 

    
    const double target_frame_time = 1.0 / 60.0;
    double last_time = hkl_get_time();

    printf("Window running. Press 'A' to start animation.\n");
    
    while (hkl_window_is_open(window)) {
        double current_time = hkl_get_time();
        double elapsed_time = current_time - last_time;

        if (elapsed_time < target_frame_time) {
            continue; 
        }
        last_time = current_time;
        
        update_animations(overlay);
        
        // --- B. Library Frame Cycle (Hides all GLFW/GL boilerplate) ---
        // This single function MUST handle:
        // 1. glfwPollEvents()
        // 2. Redrawing all widget buffers based on their current state.
        // 3. glClear(), display_buffer(), glfwSwapBuffers().
        hkl_run_frame(window); 
    }
    
    // --- 3. Cleanup ---
    hkl_destroy_window(window);
    return 0;
}




















