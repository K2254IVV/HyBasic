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
    
    // Обработка аргументов командной строки
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "--text") == 0 || strcmp(argv[i], "-t") == 0) {
                printf("Text mode enabled via command line\n");
            } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
                printf("Usage: %s [options]\n", argv[0]);
                printf("Options:\n");
                printf("  --text, -t    Enable text mode\n");
                printf("  --help, -h    Show this help\n");
                return 0;
            }
        }
    }
    
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
