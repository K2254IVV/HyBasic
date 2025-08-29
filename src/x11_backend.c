#include "hybasic.h"
#include <X11/X.h>
#include <X11/keysym.h>
#include <stdio.h>

bool x11_backend_init(HyBasicServer *server) {
    server->display = XOpenDisplay(NULL);
    if (!server->display) {
        fprintf(stderr, "Cannot open X display\n");
        return false;
    }
    
    server->screen = DefaultScreen(server->display);
    server->root = RootWindow(server->display, server->screen);
    
    // Create graphics context
    server->gc = XCreateGC(server->display, server->root, 0, NULL);
    XSetForeground(server->display, server->gc, WhitePixel(server->display, server->screen));
    XSetBackground(server->display, server->gc, BlackPixel(server->display, server->screen));
    
    // Set window manager properties
    XSetWindowAttributes attrs;
    attrs.override_redirect = True;
    XChangeWindowAttributes(server->display, server->root, CWOverrideRedirect, &attrs);
    
    // Select input events
    XSelectInput(server->display, server->root, 
                SubstructureRedirectMask | SubstructureNotifyMask |
                KeyPressMask | KeyReleaseMask |
                ButtonPressMask | ButtonReleaseMask |
                PointerMotionMask | ExposureMask);
    
    // Grab keys
    XGrabKey(server->display, XKeysymToKeycode(server->display, XK_F1), 
             AnyModifier, server->root, True, GrabModeAsync, GrabModeAsync);
    
    XSync(server->display, False);
    
    return true;
}

void x11_backend_handle_events(HyBasicServer *server) {
    XEvent event;
    
    while (XPending(server->display) > 0) {
        XNextEvent(server->display, &event);
        
        switch (event.type) {
            case KeyPress:
                input_handle_keypress(server, &event.xkey);
                break;
                
            case ButtonPress:
            case ButtonRelease:
                input_handle_mouse(server, &event.xbutton);
                break;
                
            case CreateNotify:
                printf("Window created: %lu\n", event.xcreatewindow.window);
                break;
                
            case DestroyNotify:
                printf("Window destroyed: %lu\n", event.xdestroywindow.window);
                break;
                
            case MapNotify:
                printf("Window mapped: %lu\n", event.xmap.window);
                break;
                
            case UnmapNotify:
                printf("Window unmapped: %lu\n", event.xunmap.window);
                break;
                
            case Expose:
                compositor_render(server);
                break;
        }
    }
}

void x11_backend_draw(HyBasicServer *server) {
    if (server->text_mode) {
        text_mode_draw(server);
    } else {
        // Clear screen
        XSetForeground(server->display, server->gc, BlackPixel(server->display, server->screen));
        XFillRectangle(server->display, server->root, server->gc, 0, 0, 
                      DisplayWidth(server->display, server->screen),
                      DisplayHeight(server->display, server->screen));
        
        // Draw some basic elements
        XSetForeground(server->display, server->gc, WhitePixel(server->display, server->screen));
        XDrawString(server->display, server->root, server->gc, 10, 20, 
                   "HyBasic Server - Press F1 for menu", 35);
    }
    
    XFlush(server->display);
}
