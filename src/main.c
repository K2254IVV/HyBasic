#include "hybasic.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

HyBasicServer server;

void signal_handler(int sig) {
    printf("Received signal %d, shutting down...\n", sig);
    server.running = false;
}

int main(int argc, char *argv[]) {
    printf("Starting HyBasic Server v%s\n", HYBASIC_VERSION);
    
    // Set up signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    // Initialize server
    if (!hybasic_init(&server)) {
        fprintf(stderr, "Failed to initialize HyBasic server\n");
        return 1;
    }
    
    printf("HyBasic initialized successfully\n");
    printf("Mode: %s\n", server.text_mode ? "Text" : "Graphical");
    
    // Main loop
    hybasic_run(&server);
    
    // Cleanup
    hybasic_cleanup(&server);
    
    printf("HyBasic server stopped\n");
    return 0;
}
