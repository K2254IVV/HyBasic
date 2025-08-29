#include "hybasic.h"
#include <X11/keysym.h>
#include <stdio.h>
#include <string.h>  // Добавлено для strcmp

void input_handle_keypress(HyBasicServer *server, XKeyEvent *event) {
    KeySym keysym = XLookupKeysym(event, 0);
    
    switch (keysym) {
        case XK_F1:
            // Toggle text mode
            server->text_mode = !server->text_mode;
            printf("Text mode %s\n", server->text_mode ? "enabled" : "disabled");
            break;
            
        case XK_F2:
            // Rearrange windows
            compositor_arrange_windows(server);
            break;
            
        case XK_Escape:
            server->running = false;
            break;
            
        default:
            if (server->text_mode) {
                char buffer[10];
                int count = XLookupString(event, buffer, sizeof(buffer) - 1, NULL, NULL);
                if (count > 0) {
                    buffer[count] = '\0';
                    printf("Key pressed: %s\n", buffer);
                }
            }
            break;
    }
    
    compositor_render(server);
}

void input_handle_mouse(HyBasicServer *server, XButtonEvent *event) {
    if (server->text_mode) {
        text_mode_printf(24, 0, "Mouse: X=%d Y=%d Btn=%d", 
                        event->x, event->y, event->button);
    } else {
        printf("Mouse event: X=%d Y=%d Button=%d\n", 
               event->x, event->y, event->button);
    }
}
