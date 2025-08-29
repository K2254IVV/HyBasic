#ifndef HYBASIC_H
#define HYBASIC_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>  // Добавлено для memset и других строковых функций

#define HYBASIC_VERSION "0.1.0"
#define MAX_WINDOWS 256

typedef struct {
    Window xwindow;
    int x, y;
    int width, height;
    bool visible;
    char title[256];
} HyWindow;

typedef struct {
    Display *display;
    int screen;
    Window root;
    GC gc;
    
    // Text mode properties
    bool text_mode;
    int text_cols;
    int text_rows;
    
    // Window management
    HyWindow windows[MAX_WINDOWS];
    int window_count;
    
    // Threading
    pthread_t event_thread;
    bool running;
    
    // Configuration
    int refresh_rate;
} HyBasicServer;

// Function prototypes
bool hybasic_init(HyBasicServer *server);
void hybasic_run(HyBasicServer *server);
void hybasic_cleanup(HyBasicServer *server);

// X11 backend
bool x11_backend_init(HyBasicServer *server);
void x11_backend_handle_events(HyBasicServer *server);
void x11_backend_draw(HyBasicServer *server);

// Text mode
bool text_mode_init(HyBasicServer *server);
void text_mode_draw(HyBasicServer *server);
void text_mode_printf(int row, int col, const char *format, ...);

// Compositor
void compositor_render(HyBasicServer *server);
void compositor_arrange_windows(HyBasicServer *server);

// Input
void input_handle_keypress(HyBasicServer *server, XKeyEvent *event);
void input_handle_mouse(HyBasicServer *server, XButtonEvent *event);

// Config
bool config_load(HyBasicServer *server, const char *filename);

#endif
