#include "hybasic.h"
#include <stdio.h>

void compositor_render(HyBasicServer *server) {
    x11_backend_draw(server);
    
    // Draw window manager elements
    if (server->text_mode) {
        text_mode_printf(0, 0, "HyBasic Text Mode - F1: Menu");
        text_mode_printf(1, 0, "Windows: %d", server->window_count);
    }
}

void compositor_arrange_windows(HyBasicServer *server) {
    // Simple tiling window arrangement
    int tile_width = DisplayWidth(server->display, server->screen) / server->window_count;
    
    for (int i = 0; i < server->window_count; i++) {
        if (server->windows[i].visible) {
            XMoveResizeWindow(server->display, server->windows[i].xwindow,
                             i * tile_width, 30,
                             tile_width - 2,
                             DisplayHeight(server->display, server->screen) - 35);
        }
    }
}
