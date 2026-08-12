#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Constructor function executed automatically upon dlopen / loadLibrary
__attribute__((constructor))
void my_auto_init() {
    // Target output path for package com.v2cross.shadowshare
    const char *log_path = "/sdcard/Android/data/com.v2cross.shadowshare/files/dump.txt";
    
    FILE *file = fopen(log_path, "a");
    if (file != NULL) {
        fputs("\n==========================================\n", file);
        fputs("[+] Native Library Loaded in com.v2cross.shadowshare!\n", file);
        fputs("[+] Monitoring process started...\n", file);
        fputs("==========================================\n", file);
        fclose(file);
    }
}
