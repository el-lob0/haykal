--- 
author: el-lob0
date:        HAYKAL
paging: Page %d of %d
---



 </br> 
  
   </br>
  
 </br> 
  
   </br>
  

 ```
               _   _    _ __   ___  __    _    _
              | | | |  / \\ \ / / |/ /   / \  | |
              | |_| | / _ \\ V /| ' /   / _ \ | |
              |  _  |/ ___ \| | | . \  / ___ \| |___
              |_| |_/_/   \_\_| |_|\_\/_/   \_\_____|
```
 </br> 
  
   </br>
  

### A UI framework for maximum control
 



---

# How is *Haykal* different from other UI systems ?
  
 </br> 
  
 </br> 
  
   </br>
  
### It has no OOP or inheritance
- Use of ECS (Entity Components System)
  
 </br> 
  
### Provides full control, with also simple layout logic
- Easily position and layer elements on top of each other
- Also can easily align elements horizontally or vertically
   
 </br> 
   
### Easier global theming
- No CSS, colors are variables, import fonts with their filepath
   
 </br> 
  

---

# Dependencies

## GLFW & OpenGL
   
 </br> 

- GLFW for creation of cross-platform windows and handling input
and creation of the OpenGL context
   
 </br> 

- OpenGL for drawing pixels to the screen using a texture quad
   
 </br> 
   
 </br> 

## Nib 
(github.com/el-lob0/nib) 

- My own wrapper API to GLFW


---

# Is Haykal an Immediate mode UI framework ?

Well, sort of
   
 </br> 
   
 </br> 



# Here is how code *Haykal* generally looks
   
 </br> 
   
 </br> 



## You create a window
```c 
  H_Window window = H_new_window("My App");
```
   
 </br> 


## You can give it a background color
```c 
  H_Color background = H_new_color("#1d376b");
  H_set_window_bg_color(background);
```
   
 </br> 


## You can add an element to the window
```c 
  H_Element box = H_new_box(1, 700, 100, other_color, 0 , 10 , 1, 20, 20, ABSOLUTE);
```
   
 </br> 


## Finally, run the main loop:
```c 
  while (H_main_loop_running(window)) {

    H_poll_events(); 
    H_show_frame(&window);

    H_clear_events();
  }
```

---

# Here is what you *can* do with *Haykal*
   
 </br> 
     
 </br> 

## Import a font 
     
 </br> 


```c 
  H_Font newfont64 = H_add_font("/usr/share/fonts/newfont.ttf", 64);
```
     
 </br> 

     
 </br> 


## Create a label with a transparent background 
     
 </br> 


```c 
  H_Element hello_world = H_new_label(2, "Hello World!", 0, 0, 500, 100, text_color, 64, newfont64);
```
     
 </br> 

     
 </br> 


## Create a label with a background color 
     
 </br> 


```c 
  H_Element button = H_new_button(2, "Press me.", 0, 0, 100, 90, 10, text_color, button_color, 64, newfont64);
```
     
 </br> 

     
 </br> 


## Create an axis, tie it to an element, and align other elements along said axis 
     
 </br> 


```c
  H_Element main_container = H_new_box(1, 700, 100, background, 0 , 10 , 1, 20, 20, ABSOLUTE);

  H_Axis y_axis = H_create_axis(VERTICAL, main_container, 0, 0, 5);

  H_add_to_axis(y_axis, row_1);
  H_add_to_axis(y_axis, row_2);
```

---

# The ECS system
  
</br>
  
Every visual element is represented by an ID pointing to its metadata. And even if they behave differently they all publicly have the same 
type.  
  
  </br>  
  

#### Example:

| Element IDs    | Color  | Master axis |
| -------------- | ------ | ----------- | 
| 123            | red    |    None     |
|                |        |             |
| 456            | green  | 123         |
|                |        |             |
| 789            | blue   | 123         |


---

# *Demo and Example*



</br> 
  
  
```c

int main() {

  H_Window window = H_new_window("Calculator");

/* main container, buttons, axis declaration */

/* flags initialization */

  H_monitor_resize(window.window, H_update_size);
  
  while (H_main_loop_running(window)) {

    continue_signal = false; 
    H_poll_events(); 
    H_show_frame(&window);

    if (continue_signal) { continue; } 
    animate_button(button_1, result_display, window, &flag, &frame, &continue_signal, &lag);

    H_clear_events();
  }


  return 0;
}
``` 

---

# Comments about stuff that doesn't yet work



```
 _____ _                 _           __
|_   _| |__   __ _ _ __ | | _____   / _| ___  _ __   _   _  ___  _   _ _ __
  | | | '_ \ / _` | '_ \| |/ / __| | |_ / _ \| '__| | | | |/ _ \| | | | '__|
  | | | | | | (_| | | | |   <\__ \ |  _| (_) | |    | |_| | (_) | |_| | |
  |_| |_| |_|\__,_|_| |_|_|\_\___/ |_|  \___/|_|     \__, |\___/ \__,_|_|
                                                     |___/
                      _   _             _   _
                 __ _| |_| |_ ___ _ __ | |_(_) ___  _ __
                / _` | __| __/ _ \ '_ \| __| |/ _ \| '_ \
               | (_| | |_| ||  __/ | | | |_| | (_) | | | |
                \__,_|\__|\__\___|_| |_|\__|_|\___/|_| |_|

```
