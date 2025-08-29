#include "hybasic.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bool config_load(HyBasicServer *server, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Config file not found, using defaults\n");
        return true;
    }
    
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Remove comments and trim
        char *comment = strchr(line, '#');
        if (comment) *comment = '\0';
        
        char *key = strtok(line, "=");
        char *value = strtok(NULL, "=");
        
        if (key && value) {
            // Trim whitespace
            char *end = key + strlen(key) - 1;
            while (end > key && (*end == ' ' || *end == '\t')) end--;
            *(end + 1) = '\0';
            
            end = value + strlen(value) - 1;
            while (end > value && (*end == ' ' || *end == '\t' || *end == '\n')) end--;
            *(end + 1) = '\0';
            
            if (strcmp(key, "text_mode") == 0) {
                server->text_mode = (strcmp(value, "true") == 0);
            } else if (strcmp(key, "refresh_rate") == 0) {
                server->refresh_rate = atoi(value);
            }
        }
    }
    
    fclose(file);
    return true;
}
