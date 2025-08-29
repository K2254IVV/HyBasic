#include "hybasic.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define TEXT_BUFFER_SIZE 4096

static char text_buffer[80][25]; // 80x25 text buffer
static bool text_dirty = true;

bool text_mode_init(HyBasicServer *server) {
    server->text_cols = 80;
    server->text_rows = 25;
    
    // Clear text buffer
    for (int y = 0; y < server->text_rows; y++) {
        for (int x = 0; x < server->text_cols; x++) {
            text_buffer[x][y] = ' ';
        }
    }
    
    return true;
}

void text_mode_draw(HyBasicServer *server) {
    if (!text_dirty) return;
    
    // Clear X11 display
    XSetForeground(server->display, server->gc, BlackPixel(server->display, server->screen));
    XFillRectangle(server->display, server->root, server->gc, 0, 0, 
                  DisplayWidth(server->display, server->screen),
                  DisplayHeight(server->display, server->screen));
    
    // Draw text buffer
    XSetForeground(server->display, server->gc, WhitePixel(server->display, server->screen));
    
    int char_width = 8;
    int char_height = 16;
    
    for (int y = 0; y < server->text_rows; y++) {
        for (int x = 0; x < server->text_cols; x++) {
            if (text_buffer[x][y] != ' ') {
                char str[2] = {text_buffer[x][y], '\0'};
                XDrawString(server->display, server->root, server->gc, 
                           x * char_width, (y + 1) * char_height, 
                           str, 1);
            }
        }
    }
    
    text_dirty = false;
}

void text_mode_printf(int row, int col, const char *format, ...) {
    if (row < 0 || row >= 25 || col < 0 || col >= 80) return;
    
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    int len = strlen(buffer);
    for (int i = 0; i < len && col + i < 80; i++) {
        text_buffer[col + i][row] = buffer[i];
    }
    
    text_dirty = true;
}
