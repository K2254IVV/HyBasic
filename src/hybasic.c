#include "hybasic.h"
#include <unistd.h>
#include <stdio.h>

bool hybasic_init(HyBasicServer *server) {
    memset(server, 0, sizeof(HyBasicServer));
    
    // Default settings
    server->text_mode = false;
    server->refresh_rate = 60;
    server->running = true;
    
    // Load configuration
    config_load(server, "config/hybasic.conf");
    
    // Initialize X11 backend
    if (!x11_backend_init(server)) {
        return false;
    }
    
    // Initialize text mode if enabled
    if (server->text_mode) {
        if (!text_mode_init(server)) {
            return false;
        }
    }
    
    return true;
}

void hybasic_run(HyBasicServer *server) {
    struct timespec sleep_time;
    sleep_time.tv_sec = 0;
    sleep_time.tv_nsec = 1000000000 / server->refresh_rate;
    
    while (server->running) {
        x11_backend_handle_events(server);
        compositor_render(server);
        nanosleep(&sleep_time, NULL);
    }
}

void hybasic_cleanup(HyBasicServer *server) {
    if (server->display) {
        XCloseDisplay(server->display);
    }
}
